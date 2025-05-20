# NMS Depot Downloader Wrapper

This tool provides a simple wrapper around [DepotDownloader](https://github.com/SteamRE/DepotDownloader) to allow easy downloading of specific historical versions of *No Man's Sky* (NMS) from Steam's content delivery network.

> ⚠️ **Note:** This tool does not distribute or modify any game files directly. You must own a legitimate copy of No Man's Sky on Steam to use this tool.

---

## 🚀 Features

- CLI interface to fetch a specific version of NMS.
- Automates calling Steam Depot Downloader with the correct manifest.
- Written with Linux compatibility in mind.

---

## 🧰 Requirements

- [.NET 8 runtime](https://dotnet.microsoft.com/en-us/download) (for DepotDownloader)
- [DepotDownloader](https://github.com/SteamRE/DepotDownloader) binary (must be availabe in the PATH or a path to the installation folder has to be provided)

---

## 🔧 Usage
Simply execute the program like this:
```
./nms_downloader_wrapper
```

The program should then run interactively (non interactive command line functionality will be added in the future to allow for automation)

---

## 📦 After the Download

After DepotDownloader finishes, follow these steps to make the game executable outside of Steam:

### 1. Replace `steam_api64.dll`

- Navigate to:

  ```
  <download_directory>/Binaries/
  ```

- If you launch `NMS.exe` now, it will give a **"Steam init error"**.

To bypass this:

1. Download a replacement `steam_api64.dll` from the [Goldberg Emulator Project](https://gitlab.com/Mr_Goldberg/goldberg_emulator).
2. Go to the **Releases** section.
3. Download the latest `Goldberg_Lan_Steam_Emu_<version>.zip`.
4. Extract **only** the `steam_api64.dll` file.
5. Replace the original DLL inside the `Binaries/` folder.

---

### 2. Launching the Game

Once the replacement DLL is in place, you can run:

```bash
./NMS.exe
```

**Notes:**

- **Save games** will be stored in:

  ```
  C:\Users\<YourUsername>\AppData\Roaming\HelloGames\NMS\st_<STEAMID>
  ```

- You **won’t** be able to play online.
- Your in-game name will differ from the your Steam account.
- **Controllers may not work**; use mouse and keyboard instead.

---

### 3. (For NMS v1.20 and Above only) Unpack `NMS.exe`

If you're downloading **v1.20 (Pathfinder) or newer**, the `NMS.exe` is **SteamStub-wrapped** and will redirect to Steam, prompting you to donwload the latest version or launching it.

To fix this:

1. Download the latest release of [Steamless](https://github.com/atom0s/Steamless/releases).
2. Extract and run `Steamless.exe`.
3. Locate `NMS.exe` in the `Binaries/` folder.
4. Click **Unpack File**.
5. A new file named `NMS.unpacked.exe` will appear in the same folder.
6. Delete the original `NMS.exe`.
7. Rename `NMS.unpacked.exe` to `NMS.exe`.

You can now launch the game by running `NMS.exe`

---

## ❗ Disclaimer

This tool is intended for archival, modding, and educational purposes. The tool will not work if you do not own No Man’s Sky on Steam.

---
