#![cfg_attr(
  all(not(debug_assertions), target_os = "windows"),
  windows_subsystem = "windows"
)]

use lazy_static::*;
use std::process;
use std::sync::{Arc, Mutex};

use std::io::{BufRead, BufReader, Write};
use subprocess::*;

const ERR: u128 = 107492042;

#[tauri::command]
fn close() {
  let mut p = PROC.lock().unwrap();
  println!("Closing..");
  if let Some(_) = p.poll() {
    // the process has finished
  } else {
    // it is still running, terminate it
    p.terminate().unwrap();
  }
  process::exit(0);
}

lazy_static! {
  static ref PROC: Arc<Mutex<Popen>> = Arc::new(Mutex::new(
    Popen::create(
    &[std::env::current_exe().unwrap().as_path().parent().unwrap().join("usuarios")],
    PopenConfig {
      stdout: Redirection::Pipe,
      stdin: Redirection::Pipe,
      ..Default::default()
    },
  ).expect("couldn't spawn child command")));
}

#[tauri::command]
fn display_upper_msg(msg: String) -> String {
  let p = PROC.lock().unwrap();
  let mut lines = BufReader::new(p.stdout.as_ref().unwrap()).lines();

  let i: u128 = msg.trim().replace(" ", "").replace("\n", "").parse().unwrap_or(ERR);
  if i == ERR { return format!("Error: Ingresa un número válido") }

  p.stdin.as_ref().unwrap().write_all(format!("{i}\n").as_bytes()).unwrap();

  println!("{:?}", lines);

  format!("{}", lines.next().unwrap().unwrap())
}

fn main() {
  lazy_static::initialize(&PROC);

  tauri::Builder::default()
    .invoke_handler(tauri::generate_handler![close, display_upper_msg])
    .run(tauri::generate_context!())
    .expect("error while running tauri application");
}
