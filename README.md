# GitKlient

[![CI Build](https://github.com/HamedMasafi/GitKlient/actions/workflows/main.yml/badge.svg)](https://github.com/HamedMasafi/GitKlient/actions/workflows/main.yml)
[![GitLicense](https://gitlicense.com/badge/hamedmasafi/gitklient)](https://gitlicense.com/license/hamedmasafi/gitklient)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/b4156035d6cb44bc83d31a5120662324)](https://www.codacy.com/gh/HamedMasafi/GitKlient/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=HamedMasafi/GitKlient&amp;utm_campaign=Badge_Grade)

Git gui client for KDE

## Features

- Show overlay icons on files in the Dolphin file manager
<br />
<img src="doc/screenshots/icons.png" height="400" />

- Browse files in another branch or commit and view files content 
<br />
<img src="doc/screenshots/browse.png" height="400" />

- Autocomplete on writing commit messages
<br />
<img src="doc/screenshots/autocomplete.png" height="400" />

- Graph view for commits and merges
<br />
<img src="doc/screenshots/graph.png" height="400" />

- Show changes on visual way
<br />
<img src="doc/screenshots/diff_on_changes.png" height="400" />

- See differences and conflicts and resolve them by visual tool
<br />
<img src="doc/screenshots/merge.png" height="400" />



## Build
```
cd <path/to/paoject/source>
mkdir build
cd build
cmake ..
make
sudo make install
```
