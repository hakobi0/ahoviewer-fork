# ahoviewer

A GTK image viewer, manga reader, and booru browser.

> **Note:** This is a personal fork of [ahodesuka/ahoviewer](https://github.com/ahodesuka/ahoviewer)
> with additional features and bug fixes. See [Changes in this fork](#changes-in-this-fork)
> and [Credits](#credits) below.

## Changes in this fork

### Motivation

ahoviewer has been one of my favorite pieces of software on Linux — I've been using it
since 2017 and wanted to keep using it. For the last few years, though, it had become
broken for me: it wouldn't open, or when it did it crashed constantly, and Rule34 booru
support (which used to work) had stopped working in the last version. This fork exists to
get it running again for me and for anyone who wants to keep using ahoviewer.

The following features and fixes were added on top of upstream. All development in
this fork was done with the assistance of Claude (Anthropic's AI).

### New features
* **rule34.xxx support** — added as a default booru site (Gelbooru Beta 0.2 type).
  Because the API now requires authentication, it ships preconfigured; put your
  `api_key=...&user_id=...` credentials in the site's password field.
* **Generalised Gelbooru API-key handling** — any Gelbooru-type site whose password
  contains `api_key=` now uses key-based auth (previously hardcoded to gelbooru.com).
* **Client-side tag blacklist** — Preferences → Booru Browser → *Blacklisted tags
  (space separated)*. Posts whose tags match the blacklist are hidden, and the browser
  auto-advances to the next page if an entire page is filtered out.
* **Site editor UX** — Gelbooru-type sites now show a single *API Key* field instead
  of separate Username/API Key rows.

### Bug fixes
* Fixed a blank window on a fresh/empty config (`show_all()` was never called).
* Fixed a crash when adding a site with an empty URL in the site editor.
* Fixed a crash when opening a video or any folder containing an unreadable file
  (unhandled `Gio::Error` on the image-loading worker thread).
* Fixed a crash while searching caused by a data race and use-after-free in the
  network buffer (`Booru::Curler`).

## Building

> **Note:** This fork was built and tested on **Fedora 44** only. I can't guarantee it
> will build on your system. Windows support has been dropped since I use Linux
> exclusively.

### Dependencies
* C++ Compiler that supports the C++17 standard is required. (g++ 7, clang 5)
* meson `>= 0.47.0`
* gtkmm-3.0 `>= 3.22.0`
* glibmm-2.4 `>= 2.46.0`
* libgsic++ `>= 2.6.0`
* libconfig++ `>= 1.4`
* libcurl `>= 7.58.0`
* libxml2
* gstreamer-1.0 `optional`
    * gst-plugins-bad `runtime`
    * gst-plugins-base `runtime`
    * gst-plugins-good `runtime`
    * gst-plugins-vpx `runtime`
    * gst-plugins-libav `runtime`
    * gst-plugins-gtk `runtime`
* libpeas `>=1.22.0` `optional`
* libsecret `optional`
    * gnome-keyring `runtime`
* libunrar `optional`
* libzip `optional`

Package names may vary depending on your distro and package manager.

### Compiling and Installing
```
meson setup build
meson compile -C build
sudo meson install -C build
```

If you don't want to compile with debug symbols replace the first command with:
```
meson setup build --buildtype=release
```

## Usage

    ahoviewer

or

    ahoviewer file[.zip|.rar|.mp4|.webm|.(supported gdk-pixbuf file)]

### Screenshot
![Booru Browser](https://user-images.githubusercontent.com/1155344/91631124-e4bd4280-e99c-11ea-9432-72194d9b7aeb.gif)

### Plugins
Some example and usable plugins can be found at [ahodesuka/ahoviewer-plugins](https://github.com/ahodesuka/ahoviewer-plugins)

Plugins should be installed into `$XDG_DATA_HOME/ahoviewer/plugins`, `$XDG_DATA_HOME` is `~/.local/share` by default.
This is `%LOCALAPPDATA%\ahoviewer\plugins` on Windows.
The .typelib file that is compiled must be installed via `ninja install`, or you will need to set the
`GI_TYPELIB_PATH` environment variable to the directory where it is located after compilation.

### FAQ
1. **My password/API key is not saved after restarting ahoviewer**
   * See my comment in [issue #20](https://github.com/ahodesuka/ahoviewer/issues/20#issuecomment-157997909)
2. **I get *No results found* on Danbooru**
   * Danbooru only allows normal users to use 2 tags, if you have `Maximum post rating` set to anything other than `Explicit` it will count as one tag.
3. **I wish to use a proxy with ahoviewer**
   * This can be done by setting the `http_proxy` and `https_proxy` environment variables respectively, this works on both GNU/Linux and Windows.  See my comment in [issue #61](https://github.com/ahodesuka/ahoviewer/issues/61#issuecomment-354694187) for details.

#### Default Keybindings
| Function                                  | Key             |
| ----------------------------------------- | --------------- |
| Open File                                 | Control+o       |
| Open Preferences                          | p               |
| Close local image list or booru tab       | Control+w       |
| Quit                                      | Control+q       |
| Fullscreen                                | f               |
| Toggle Manga Mode                         | g               |
| Set Auto Fit Mode                         | a               |
| Set Fit Width Mode                        | w               |
| Set Fit Height Mode                       | h               |
| Set Manual Zoom Mode                      | m               |
| Zoom In                                   | Control+=       |
| Zoom Out                                  | Control+-       |
| Reset Zoom                                | Control+0       |
| Toggle Menubar                            | Control+m       |
| Toggle Statusbar                          | Control+b       |
| Toggle Scrollbars                         | Control+l       |
| Toggle Thumbnail bar (local image list)   | t               |
| Toggle Booru Browser                      | b               |
| Hide all (hides the above widgets)        | i               |
| Next Image                                | Page Down       |
| Previous Image                            | Page Up         |
| First Image                               | Home            |
| Last Image                                | End             |
| Toggle Slideshow                          | s               |
| New Booru Browser Tab                     | Control+t       |
| Save Booru Browser Tab                    | Control+Shift+s |
| Save Image                                | Shift+s         |
| Save Image As                             | Control+s       |
| Delete Image                              | Shift+Delete    |
| Open Booru Post in Web Browser            | Control+Shift+o |
| Copy Booru Post URL to Clipboard          | Control+y       |
| Copy Booru Image URL to Clipboard         | y               |
| Copy Booru Image Data to Clipboard        | Control+Shift+y |

## Credits

* **[ahoka / ahodesuka](https://github.com/ahodesuka)** — original author and creator of
  ahoviewer. All credit for the application itself goes to them. The original project is
  at [ahodesuka/ahoviewer](https://github.com/ahodesuka/ahoviewer).
* **Claude (Anthropic's AI)** — assisted with the additional features and bug fixes listed
  in [Changes in this fork](#changes-in-this-fork).

ahoviewer is released under the [MIT License](LICENSE); this fork retains that license and
the original copyright notice.
