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
        dep::colocar_dependencia();

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

#[tauri::command]
fn handle(msg: &str) -> String {
    let p = PROC.lock().unwrap();
    let mut lines = BufReader::new(p.stdout.as_ref().unwrap()).lines();

    p.stdin
        .as_ref()
        .unwrap()
        .write_all(format!("{msg}\n").as_bytes())
        .unwrap();

    loop {
        if let Some(Ok(s)) = lines.next() {
            let s: String = s.into();
            return smartstring::alias::String::from(s.replace(":nl:", "</br>"));
        }
    }
}

fn main() {
    lazy_static::initialize(&PROC);

    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![close, handle])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
