use directories::*;
use include_dir::{include_dir, Dir};
use lazy_static::*;
use smartstring::alias::String;
use std::fs::{create_dir, create_dir_all, OpenOptions};
use std::io::Write;
use std::path::PathBuf;

#[cfg(any(target_os = "macos", target_os = "linux"))]
use std::fs;

#[cfg(any(target_os = "macos", target_os = "linux"))]
use std::os::unix::fs::PermissionsExt;

#[cfg(any(target_os = "macos", target_os = "linux"))]
const BYTES: &[u8] = include_bytes!("./../../cpp/usuarios");

static USERS: Dir<'_> = include_dir!("./users");

#[cfg(any(target_os = "windows"))]
const BYTES: &[u8] = include_bytes!("./../../cpp/usuarios.exe");

lazy_static! {
  pub static ref PATH: PathBuf = {
    let p = ProjectDirs::from(
      "org",          // qualifier
      "SBanco",       // organization
      "MiBanco"       // application
    ).unwrap();

    let p = PathBuf::from(p.data_local_dir());

    if !&p.exists() {
        create_dir_all(&p).unwrap();
    };

    p
  };

  pub static ref PATH_BACKEND: PathBuf = {
    lazy_static::initialize(&PATH);
    PATH.join("usuarios")
  };

  pub static ref PATH_USERS: PathBuf = {
    lazy_static::initialize(&PATH);
    PATH.join("users")
  };
}

pub fn colocar_iniciales() {
    if !PATH_USERS.exists() {
        create_dir(PATH_USERS.as_path()).unwrap();
        USERS.extract(PATH_USERS.as_path()).unwrap();
    }
}

pub fn obtener_usuarios() -> String {
    let mut names = String::new();
    for file in PATH_USERS.as_path().read_dir().unwrap() {
        names.push_str(file.as_ref().unwrap().file_name().to_str().unwrap());
        names.push_str("*");
        names.push_str(file.as_ref().unwrap().path().as_os_str().to_str().unwrap());
        names.push_str("*");
        names.push_str(";");
    }

    names
}

pub fn colocar_dependencia() {
    let mut file = OpenOptions::new()
        .write(true)
        .append(false)
        .create(true)
        .truncate(true)
        .open(PATH_BACKEND.as_path())
        .unwrap();

    file.write_all(BYTES).unwrap();

    #[cfg(any(target_os = "macos", target_os = "linux"))]
    {
        let mut perms = fs::metadata(PATH_BACKEND.as_path()).unwrap().permissions();
        perms.set_mode(0o744);
        std::fs::set_permissions(PATH_BACKEND.as_path(), perms).unwrap();
    }
}
