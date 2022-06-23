set(GIT_KLIENT_ICONS
  16-apps-gitklient.png
  22-apps-gitklient.png
  32-apps-gitklient.png
  48-apps-gitklient.png
  sc-apps-gitklient.svgz

  sc-actions-diff-keep-their.svg
  sc-actions-diff-keep-mine.svg

  sc-actions-diff-keep-mine-before-their.svg
  sc-actions-diff-keep-their-before-mine.svg
  sc-actions-diff-keep-their-file.svg
  sc-actions-diff-keep-mine-file.svg

  sc-actions-git-status-added.svg
  sc-actions-git-status-ignored.svg
  sc-actions-git-status-modified.svg
  sc-actions-git-status-removed.svg
  sc-actions-git-status-unknown.svg
  sc-actions-git-status-update.svg
  sc-actions-git-status-renamed.svg

  sc-actions-diff-goto-next-diff.svg
  sc-actions-diff-goto-prev-diff.svg

  sc-actions-git-branch.svg
  sc-actions-git-commit.svg
  sc-actions-git-overview.svg
  sc-actions-git-remote.svg
  sc-actions-git-tag.svg
  sc-actions-git-stash.svg
  sc-actions-git-submodules.svg

  sc-actions-gitklient-changedfiles.svg
)

ecm_install_icons(ICONS ${GIT_KLIENT_ICONS} DESTINATION ${KDE_INSTALL_ICONDIR} THEME hicolor)
