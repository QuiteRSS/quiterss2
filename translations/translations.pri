INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

TRANSLATIONS += \
    $$PWD/quiterss_ru.ts \
#    $$PWD/quiterss_de.ts \
#    $$PWD/quiterss_es.ts \
#    $$PWD/quiterss_fr.ts \
#    $$PWD/quiterss_hu.ts \
#    $$PWD/quiterss_sv.ts \
#    $$PWD/quiterss_sr.ts \
#    $$PWD/quiterss_nl.ts \
#    $$PWD/quiterss_fa.ts \
#    $$PWD/quiterss_it.ts \
#    $$PWD/quiterss_zh_CN.ts \
#    $$PWD/quiterss_uk.ts \
#    $$PWD/quiterss_cs.ts \
#    $$PWD/quiterss_pl.ts \
#    $$PWD/quiterss_ja.ts \
#    $$PWD/quiterss_ko.ts \
#    $$PWD/quiterss_pt_BR.ts \
#    $$PWD/quiterss_lt.ts \
#    $$PWD/quiterss_zh_TW.ts \
#    $$PWD/quiterss_el_GR.ts \
#    $$PWD/quiterss_tr.ts \
#    $$PWD/quiterss_ar.ts \
#    $$PWD/quiterss_sk.ts \
#    $$PWD/quiterss_tg_TJ.ts \
#    $$PWD/quiterss_pt_PT.ts \
#    $$PWD/quiterss_vi.ts \
#    $$PWD/quiterss_ro_RO.ts \
#    $$PWD/quiterss_fi.ts \
#    $$PWD/quiterss_gl.ts \
#    $$PWD/quiterss_bg.ts \
#    $$PWD/quiterss_hi.ts \

isEmpty(QMAKE_LRELEASE) {
  Q_OS_WIN:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
  else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}

updateqm.input = TRANSLATIONS
updateqm.output = $$DESTDIR/translations/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm $$DESTDIR/translations/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm
