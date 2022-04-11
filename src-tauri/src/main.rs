#![cfg_attr(
    all(not(debug_assertions), target_os = "windows"),
    windows_subsystem = "windows"
)]

mod dep;

use lazy_static::*;
use smartstring::alias::String;
use std::io::{BufRead, BufReader, Write};
use std::sync::{Arc, Mutex};
use subprocess::*;

#[tauri::command]
fn close(app: tauri::AppHandle) {
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

        Arc::new(Mutex::new(
            Popen::create(
                &[dep::PATH.join("usuarios")],
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

lazy_static! {
    static ref ERROR: String = String::from("<h1>Ups, something went wrong</h1>");
}

#[tauri::command]
fn handle(msg: &str) -> String {
    if msg.contains("FILES_PLS") {
        write_to_stdin(&dep::obtener_usuarios(), false);
        return String::new();
    }

    if let Some(Ok(s)) = write_to_stdin(msg, true) {
        let s: String = s.into();

        if s.contains("FILES_PLS") {
            handle(&dep::obtener_usuarios())
        } else {
            s
        }
    } else {
        ERROR.clone()
    }
}

fn main() {
    println!("Init...");
    lazy_static::initialize(&PROC);
    handle("FILES_PLS");

    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![close, handle])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
