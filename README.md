# RomMArch 3DS

A Nintendo 3DS-focused modification of RetroArch that adds RomM server
integration for ROM library browsing, downloads, and save synchronization.

This project is under development.

## Credits

RomMArch builds on [RetroArch](https://github.com/libretro/RetroArch),
developed by the Libretro project and its contributors.

RomMArch-specific modifications and integration are maintained by
[thezphynx](https://github.com/thezphynx).

Additional projects that make this work possible:

- [RomM](https://github.com/rommapp/romm): server and API.
- [devkitPro](https://devkitpro.org/): Nintendo 3DS build toolchain.
- [libctru](https://github.com/devkitPro/libctru): Nintendo 3DS homebrew library.

RomMArch is an independent modification and does not imply endorsement
by these projects.

## Upstream documentation and licenses

The original RetroArch README is preserved in
[README.upstream.md](README.upstream.md). Its download and support links
refer to upstream RetroArch.

See [COPYING](COPYING) for the included GNU GPL v3 license text.
Existing copyright notices, author credits, and component licenses
are retained.

## Configuration

Personal settings are stored on the 3DS in
`sdmc:/retroarch/rommarch.cfg`.

Do not publish your API token or personal configuration file.

### Current RomMArch menu layout

The RomMArch-specific menu is organized around:

- **RomM Configuration** - RomM server address and API token.
- **HTTP Proxy** - enable/disable the proxy and configure its address and port.
- **ROM Directory** - local ROM/download location used by RomMArch.
- **Save Directory** - local save location.
- **Rom Library** - platform-first browsing of the configured RomM server.
- **Save Synchronization** - per-platform save directory configuration and manual sync.
  The submenu currently contains **Configure Core Save Directories**,
  **Automatic Synchronization**, and **Initiate Manual Sync**.

The RomMArch configuration is intentionally separate from `retroarch.cfg`.
RomMArch server, proxy, library and save-sync values are stored in
`sdmc:/retroarch/rommarch.cfg`.

### HTTP proxy and mobile-hotspot connectivity

RomMArch includes configurable HTTP CONNECT proxy support for networks that
require a proxy to reach the Internet. A primary use case is connecting a 3DS
to a phone-based hotspot/proxy while away from normal Wi-Fi, including access
to RetroAchievements from compatible RomMArch-integrated emulator builds.

The RomMArch proxy settings are stored in `sdmc:/retroarch/rommarch.cfg` as:

- `http_proxy_enabled`
- `http_proxy_host`
- `http_proxy_port`

If no custom values have been saved, the frontend currently defaults to
`192.168.49.1` and port `8080`. Always use the host and port reported by the
proxy/hotspot software you are actually running.

The proxy must be configured successfully in the stock Nintendo 3DS Internet
settings before relying on it in RomMArch. Nintendo documents the system proxy
menu in its [3DS proxy settings guide](https://en-americas-support.nintendo.com/app/answers/detail/a_id/75/~/how-to-change-proxy-server-settings-on-nintendo-3ds-family-systems):

1. Start the phone hotspot/proxy and connect the 3DS to the network it exposes.
2. On the 3DS open **System Settings -> Internet Settings -> Connection
   Settings**, select the active connection, choose **Change Settings**, move to
   the next page and open **Proxy Settings**.
3. Enable the proxy, enter the proxy server/address and port, save the
   connection, then run the stock 3DS **Connection Test**. Do not continue until
   the system connection test succeeds.
4. Open RomMArch and enter the same address and port under **HTTP Proxy**, then
   enable the proxy toggle.
5. Use RomMArch's **Test Connection** function to verify application-level
   connectivity.

When returning to a normal Wi-Fi connection that does not require a proxy,
disable the RomMArch proxy toggle. The active Nintendo 3DS connection profile
should likewise have proxy settings enabled only when that network requires
them. Keeping a separate 3DS connection profile for the proxy-backed hotspot
can make switching less error-prone.

After a prolonged sleep/network interruption, fully close and relaunch
RomMArch before retrying network-dependent features. A simple wait/retry is not
considered a reliable recovery procedure.

[Conduit](https://github.com/KingTChawla/Conduit) is one example of third-party
Android software that exposes a Wi-Fi Direct network and HTTP CONNECT proxy
(the project's documented default is `192.168.49.1:8080`). Conduit is not
bundled with RomMArch, and the projects are not affiliated with or endorsed by
one another. Conduit is distributed by its own authors under GPL-3.0; refer to
its repository for its license and usage terms. HTTP CONNECT proxies carry TCP
traffic; UDP-dependent networking such as many real-time multiplayer protocols
is outside the scope of this feature.

### Save synchronization

**Manual synchronization is the supported save-sync mode in the current public
package.** Open **Save Synchronization -> Configure Core Save Directories** to
enable the desired RomM platforms and map each platform to the directory used
by its emulator/core. Then use **Initiate Manual Sync**. When the network is not
available, the menu reports that state and leaves the saved per-platform
configuration visible for offline inspection.

Save reconciliation uses RomM's server `updated_at` value to determine remote
chronology instead of relying on the Nintendo 3DS clock. A downloaded
timestamped/archival server save is normalized locally to the canonical
`<Title>.srm` filename. Uploads target the canonical remote `<Title>.srm`; older
timestamped/history files on the server are left untouched. When both sides
have changed, the conflict-resolution menu is used instead of silently
overwriting either copy.

> **Developer preview - Automatic Synchronization**
>
> The **Automatic Synchronization** toggle is present in the frontend for
> developer testing only. It is **not a supported end-user feature in the
> current package and should be left OFF/ignored by end users**. The currently
> released emulator CIA package does not yet contain the updated
> RomMArch-integrated emulator builds required to ship this feature. Automatic
> per-ROM launch/close/replacement synchronization is intended for a future
> release alongside an updated emulator CIA package.

### RetroAchievements menu performance

On Nintendo 3DS, the Achievements menu no longer eagerly decodes badge textures
that RGUI does not display. The first visit may still perform normal
RetroAchievements/network initialization, but subsequent openings avoid the
previous repeated badge-allocation cost and should respond substantially
faster.

### Quick Menu Reset

The Nintendo 3DS **Quick Menu -> Reset** path has been changed to avoid an
indefinite frontend hang seen with static cores. Reset now reloads the current
content inside the already-running integrated core instead of entering the
problematic direct core-reset path. This also avoids accidentally launching or
installing a separate core CIA while restarting the current game.

## Feedback

Report RomMArch-specific issues in
[this repository](https://github.com/thezphynx/RomMArch/issues).

## Building the Nintendo 3DS frontend

The development setup uses Windows with the devkitPro MSYS terminal.

Recorded toolchain versions:

- devkitARM r68
- ARM GCC 16.1.0
- libctru 2.7.0
- 3dstools 1.3.1
- picasso 2.7.2

The Makefile expects `DEVKITPRO` and `DEVKITARM` to be set.
The repository includes the bannertool and makerom executables.

From the repository root:

```bash
make -f Makefile.ctr clean
make -f Makefile.ctr HAVE_STATIC_DUMMY=1 USE_CTRULIB_2=1 -j2
```

The default output filenames are `retroarch_3ds.3dsx` and
`retroarch_3ds.cia`.

This builds the RomMArch frontend with a dummy core. It does not
include a playable emulator core. Gameplay requires separate core
builds; instructions for those are not yet provided here.

Personal configuration and generated binaries are excluded from
new Git additions. Compiled releases can be distributed through
GitHub Releases.

## Building a complete package

With the build prerequisites installed, run from the repository root:

```bash
make -f Makefile.ctr clean
make -f Makefile.ctr HAVE_STATIC_DUMMY=1 USE_CTRULIB_2=1 -j2
bash package-rommarch.sh
```

Packaging requires curl, sha256sum, and 7-Zip. On Windows, the script
also checks the standard `C:\Program Files\7-Zip\7z.exe` location.

The script automatically downloads the RetroArch 1.17.0 Nintendo 3DS
CIA distribution, verifies its pinned SHA-256 checksum, and combines
its supporting folders and prebuilt cores with the compiled RomMArch
frontend. No manual download or selection of cores is required.

The completed package is written to a fresh
`RomMArch-package.XXXXXX` folder in your home directory. It includes
installation instructions and the supporting archive's source URL
and checksum.

### Core compatibility baseline

RomMArch intentionally packages Nintendo 3DS emulator cores and
supporting files from RetroArch 1.17.0. Later core builds tested by
the maintainer exhibited touchscreen crashes that remain unresolved
in this project. RetroArch 1.17.0 remains the supported baseline until a newer version is
explicitly validated.

Not every bundled core has been individually tested. The bundled core
executables retain their original RetroArch frontend; the RomMArch
submenu belongs to the separately built frontend.

### Installing the CIA package

1. Copy the supplied `retroarch` folder to the SD card root, producing
   `sdmc:/retroarch/`.
2. Copy `retroarch_3ds.cia` onto the SD card and install it using FBI.
3. Open RomMArch and launch the desired core through the frontend,
   allowing it to install the bundled core on first launch.
4. Configure your server, API token, ROM directory, and save sync.

The `.3dsx` frontend is an alternative launch format and is not required
for CIA installation.

Back up an existing installation before copying. Merging folders can
leave extra newer core installers behind. Existing RetroArch settings
may override default directory paths.
