#![cfg_attr(
    all(not(debug_assertions), target_os = "windows"),
    windows_subsystem = "windows"
)]

mod dep;

use lazy_static::*;
use lcy_lib::{cypher_bytes, decipher_bytes};
use smartstring::alias::String;
use std::env::set_current_dir;
use std::fs::OpenOptions;
use std::io::{BufRead, BufReader, Write};
use std::path::PathBuf;
use std::sync::{Arc, Mutex};
use subprocess::*;

#[cfg(target_os = "macos")]
use tauri::{AboutMetadata, Menu, MenuEntry, MenuItem, Submenu};

#[tauri::command]
fn close(app: tauri::AppHandle<tauri::Wry>) {
    println!("Closing app...");
    {
        let mut p = PROC.lock().unwrap();
        if p.poll().is_none() {
            p.terminate().unwrap();
        }
    }

    {
        let _d2 = dep::PATH.deref();
        let _d3 = PROC.deref();
    }

    app.exit(0);
}

lazy_static! {
    static ref PROC: Arc<Mutex<Popen>> = {
        lazy_static::initialize(&dep::PATH);
        lazy_static::initialize(&dep::PATH_USERS);
        lazy_static::initialize(&dep::PATH_BACKEND);
        dep::colocar_dependencia();
        dep::colocar_iniciales();

        set_current_dir(&*dep::PATH.clone()).unwrap();

        Arc::new(Mutex::new(
            Popen::create(
                &[
                    dep::PATH.join("usuarios"),
                    PathBuf::from(&format!("{}", dep::PATH.join("file").display())),
                ],
                PopenConfig {
                    stdout: Redirection::Pipe,
                    stdin: Redirection::Pipe,
                    ..Default::default()
                },
            )
            .expect("couldn't spawn child command"),
        ))
    };
}

fn send(msg: &str) {
    {
        let mut file = OpenOptions::new()
            .read(false)
            .write(true)
            .create(true)
            .truncate(true)
            .open(dep::PATH.join("file"))
            .unwrap();

        let cifrado = cypher_bytes([msg, "\n"].concat().as_bytes().to_vec());

        file.write_all(&cifrado).unwrap();
    }
}

fn receive() -> String {
    let contenidos = std::fs::read(dep::PATH.join("file")).unwrap();

    let descifrado = decipher_bytes(contenidos);

    std::string::String::from_utf8(descifrado).unwrap().into()
}

fn write_to_stdin(msg: &str, read: bool) -> Option<std::io::Result<std::string::String>> {
    let p = PROC.lock().unwrap();

    let lines = if read {
        Some(BufReader::new(p.stdout.as_ref().unwrap()).lines())
    } else {
        None
    };

    p.stdin
        .as_ref()
        .unwrap()
        .write_all(format!("{}\n", msg).as_bytes())
        .unwrap();

    if read {
        lines.unwrap().next()
    } else {
        None
    }
}

#[tauri::command]
fn handle(msg: &str) -> String {
    println!("Beginning of NOTIFICATION");
    println!("\tContents:\n\n\t\t{}\n", msg);

    send(msg);
    let result = write_to_stdin("NOTIFICATION", msg != dep::obtener_usuarios());
    println!("\tReceived: {:?}", result);

    if let Some(Ok(_)) = result {
        let s = receive();
        println!("\tResponse contents:\n\n\t\t{}\n", s);
        s
    } else {
        String::from("ERROR")
    }
}

#[tauri::command]
fn handle_multiple(msgs: Vec<&str>) -> String {
    for msg in msgs.iter().enumerate() {
        println!("Beginning of NOTIFICATION");
        println!("\tContents:\n\n\t\t{}\n", msg.1);

        let msg: (usize, &&str) = msg;

        send(msg.1);
        let result = write_to_stdin("NOTIFICATION", true);
        println!("\tReceived: {:?}", result);
        if let Some(Ok(_)) = result {
            if msg.0 == msgs.len() - 1 {
                let s = receive();
                println!("\ttResponse contents:\n\n\t\t{}\n", s);
                return s;
            }
        } else if msg.0 == msgs.len() - 1 {
            return String::from("ERROR");
        }
    }

    String::from("ERROR")
}

fn main() {
    println!("Init...");
    initialize(&PROC);
    handle(&dep::obtener_usuarios());

    #[cfg(target_os = "macos")]
    {
        let ctx = tauri::generate_context!();

        tauri::Builder::default()
            .invoke_handler(tauri::generate_handler![close, handle, handle_multiple])
            .menu(Menu::with_items([
                #[cfg(target_os = "macos")]
                MenuEntry::Submenu(Submenu::new(
                    "SuperBanco",
                    Menu::with_items([
                        MenuItem::About(ctx.package_info().name.clone(), AboutMetadata::default())
                            .into(),
                        MenuItem::Separator.into(),
                        MenuItem::Services.into(),
                        MenuItem::Separator.into(),
                        MenuItem::Hide.into(),
                        MenuItem::HideOthers.into(),
                        MenuItem::ShowAll.into(),
                        MenuItem::Separator.into(),
                        MenuItem::Quit.into(),
                    ]),
                )),
                MenuEntry::Submenu(Submenu::new(
                    "Edit",
                    Menu::with_items([
                        MenuItem::Undo.into(),
                        MenuItem::Redo.into(),
                        MenuItem::Separator.into(),
                        MenuItem::Cut.into(),
                        MenuItem::Copy.into(),
                        MenuItem::Paste.into(),
                        #[cfg(not(target_os = "macos"))]
                        MenuItem::Separator.into(),
                        MenuItem::SelectAll.into(),
                    ]),
                )),
            ]))
            .run(tauri::generate_context!())
            .expect("error while running tauri application");
    }

    // No menú on windows
    #[cfg(not(target_os = "macos"))]
    {
        tauri::Builder::default()
            .invoke_handler(tauri::generate_handler![close, handle, handle_multiple])
            .run(tauri::generate_context!())
            .expect("error while running tauri application");
    }
}
