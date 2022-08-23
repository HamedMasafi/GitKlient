# GitKlient

[![CI Build](https://github.com/HamedMasafi/GitKlient/actions/workflows/main.yml/badge.svg)](https://github.com/HamedMasafi/GitKlient/actions/workflows/main.yml)
[![GitLicense](https://gitlicense.com/badge/hamedmasafi/gitklient)](https://gitlicense.com/license/hamedmasafi/gitklient)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/b4156035d6cb44bc83d31a5120662324)](https://www.codacy.com/gh/HamedMasafi/GitKlient/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=HamedMasafi/GitKlient&amp;utm_campaign=Badge_Grade)

Git gui client for KDE

## Build

Install Qt via online installer or your distru's package manager
```
sudo apt install qt5-default qt5-qmake qt5-qmake-bin libqt5core5a libqt5gui5 libqt5concurrent5
```

Install required packages
```
sudo apt install libkf5config-dev libkf5configwidgets5 libkf5configwidgets-dev libkf5coreaddons-dev libkf5crash-dev libkf5dbusaddons-dev libkf5doctools-dev kdoctools-dev libkf5i18n-dev libkf5xmlgui-dev libkf5kio-dev libkf5kiocore5 libkf5kiogui5 libkf5kiowidgets5 libkf5textwidgets-dev libkf5texteditor-dev cmake make extra-cmake-modules gettext libkf5syntaxhighlighting-dev libkf5syntaxhighlighting-data libkf5syntaxhighlighting-tools
```

Navigate to source dir and do these steps
```
mkdir build
cd build
cmake ..
make -j 8
make install
```

## Features

<details>
    <summary>Show overlay icons on files in the Dolphin file manager</summary>
    <img src="doc/screenshots/icons.png" height="400" />
</details>

<details>
    <summary>Browse files in another branch or commit and view files content </summary>
    <img src="doc/screenshots/browse.png" height="400" />
</details>
<details>
    <summary>Autocomplete on writing commit messages</summary>
    <img src="doc/screenshots/autocomplete.png" height="400" />
</details>
<details>
    <summary>Graph view for commits and merges</summary>
    <img src="doc/screenshots/graph.png" height="400" />
</details>
<details>
    <summary>Show changes on visual way</summary>
    <img src="doc/screenshots/diff_on_changes.png" height="400" />
</details>
<details>
    <summary>See differences and conflicts and resolve them by visual tool</summary>
    <img src="doc/screenshots/merge.png" height="400" />
</details>

