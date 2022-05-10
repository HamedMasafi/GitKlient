# GitKlient

[![CI Build](https://github.com/HamedMasafi/GitKlient/actions/workflows/main.yml/badge.svg)](https://github.com/HamedMasafi/GitKlient/actions/workflows/main.yml)
[![GitLicense](https://gitlicense.com/badge/hamedmasafi/gitklient)](https://gitlicense.com/license/hamedmasafi/gitklient)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/b4156035d6cb44bc83d31a5120662324)](https://www.codacy.com/gh/HamedMasafi/GitKlient/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=HamedMasafi/GitKlient&amp;utm_campaign=Badge_Grade)

Git gui client for KDE

## Features

- Show overlay icons on files in the Dolphin file manager ([Screenshot](doc/screenshots/icons.png))
- Browse files in another branch or commit and view files content ([Screenshot](doc/screenshots/browse.png))
- Autocomplete on writing commit messages ([Screenshot](doc/screenshots/autocomplete.png))
- Graph view for commits and merges ([Screenshot](doc/screenshots/graph.png))
- Show changes on visual way ([Screenshot](doc/screenshots/diff_on_changes.png))
- See differences and conflicts and resolve them by visual tool ([Screenshot](doc/screenshots/merge.png))


## Build
```
cd <path/to/paoject/source>
mkdir build
cd build
cmake ..
make
sudo make install
```
