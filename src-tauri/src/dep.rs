use directories::*;
use fs_extra::dir::remove;
use lazy_static::*;
use std::fs::remove_file;
use std::fs::create_dir_all;
use std::fs::OpenOptions;
use std::io::Write;
use std::path::PathBuf;

#[cfg(any(target_os = "macos", target_os = "linux"))]
use std::os::unix::fs::PermissionsExt;

const BYTES: &[u8] = include_bytes!("./../usuarios.exe");

lazy_static! {
    pub static ref PATH: PathBuf = {
        let p = ProjectDirs::from(
            "org",          // qualifier
            "SBanco",       // organization
            "MiBanco"       // application
        ).unwrap();

        PathBuf::from(p.data_local_dir())
    };
}

pub fn colocar_dependencia() {
  let usuarios_path = PATH.join("usuarios");

  if !PATH.exists() {
    create_dir_all(PATH.clone()).unwrap()
  }

  #[cfg(any(target_os = "macos", target_os = "linux"))]
  {
    let mut perms = fs::metadata(usuarios_path.clone()).unwrap().permissions();
    perms.set_mode(0o744);
    std::fs::set_permissions(usuarios_path.clone(), perms).unwrap();
  }
}
