#![cfg_attr(
  all(not(debug_assertions), target_os = "windows"),
  windows_subsystem = "windows"
)]

mod dep;

use lazy_static::*;
use std::io::{BufRead, BufReader, Write};
use std::process;
use std::sync::{Arc, Mutex};
use subprocess::*;

macro_rules! dbgf {
  ($val:expr) => {
    match $val {
      tmp => {
        eprintln!("[{}:{}] {}", file!(), line!(), $val);
        tmp
      }
    }
  };
  ($tag:expr, $val:expr $(,)?) => {
    match $val {
      tmp => {
        eprintln!("[{}:{}] ({:#?}) {}", file!(), line!(), $tag, $val);
        tmp
      }
    }
  };
}

#[tauri::command]
fn close() {
  let mut p = PROC.lock().unwrap();
  println!("Closing..");
  if p.poll().is_some() {
    // the process has finished
  } else {
    // it is still running, terminate it
    p.terminate().unwrap();
  }
  process::exit(0);
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
fn display_upper_msg(msg: String) -> String {
  dep::colocar_dependencia();
  let p = PROC.lock().unwrap();
  let mut lines = BufReader::new(p.stdout.as_ref().unwrap()).lines();

  p.stdin
    .as_ref()
    .unwrap()
    .write_all(format!("{msg}\n").as_bytes())
    .unwrap();

  loop {
    dbgf!("Trying to get value...");
    if let Some(Ok(s)) = lines.next() {
      return s.replace(":nl:", "</br>");
    }
  }
}

fn main() {
  lazy_static::initialize(&PROC);

  tauri::Builder::default()
    .invoke_handler(tauri::generate_handler![close, display_upper_msg])
    .run(tauri::generate_context!())
    .expect("error while running tauri application");
}
