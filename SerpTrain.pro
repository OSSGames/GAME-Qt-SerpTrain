TEMPLATE = app
TARGET = SerpTrain
QT += core \
    gui \
    xml \
    network
HEADERS += SConfigDlg.h \
    SChgLayerSizeDlg.h \
    SEdtLevelDlg.h \
    SEdtLayerDlg.h \
    SLayerGraphics.h \
    SArray.h \
    SBoardGoalSettingsWidget.h \
    SBoardTunnelSettingsWidget.h \
    SOpenLevel.h \
    VersionInformation.h \
    SHttpGet.h \
    SHttpPost.h \
    SBoardSwitchTurn.h \
    SBoard2Turn.h \
    SGraphicsEdtColors.h \
    SEdtSettingsDlg.h \
    SGraphicsTunnelMarker.h \
    SXMLConv.h \
    SXMLElement.h \
    SXMLDoc.h \
    SGraphicsEdtSheet.h \
    SLayer.h \
    SColor.h \
    SPathGoalTurn.h \
    SGraphicsStraightPart.h \
    SGraphicsGoalTurn.h \
    SBoardGoal.h \
    enum.h \
    SBoardNone.h \
    SXMLParse.h \
    SGraphicsSwitchControl.h \
    SPathTunnel.h \
    SBoardTunnel.h \
    SGraphicsTunnel.h \
    SBoardSwitch.h \
    SBoardStraight.h \
    SGameBoard.h \
    SBoardTurn.h \
    SException.h \
    SBall.h \
    SPathTurn.h \
    SPathStraight.h \
    SPath.h \
    SGraphicsStraight.h \
    SGraphicsTurn.h \
    SConfig.h \
    SGraphicsRail.h \
    SGraphicsTile.h \
    SGraphicsItem.h \
    SBoardTile.h
SOURCES += SConfigDlg.cpp \
    SChgLayerSizeDlg.cpp \
    SEdtLevelDlg.cpp \
    SEdtLayerDlg.cpp \
    SLayerGraphics.cpp \
    SBoardGoalSettingsWidget.cpp \
    SBoardTunnelSettingsWidget.cpp \
    SOpenLevel.cpp \
    VersionInformation.cpp \
    SHttpGet.cpp \
    SHttpPost.cpp \
    SBoardSwitchTurn.cpp \
    SBoard2Turn.cpp \
    SGraphicsEdtColors.cpp \
    SEdtSettingsDlg.cpp \
    SGraphicsTunnelMarker.cpp \
    SXMLConv.cpp \
    SXMLElement.cpp \
    SXMLDoc.cpp \
    SLayer.cpp \
    SColor.cpp \
    SPathGoalTurn.cpp \
    SGraphicsStraightPart.cpp \
    SGraphicsGoalTurn.cpp \
    SBoardGoal.cpp \
    SBoardNone.cpp \
    SXMLParse.cpp \
    SGraphicsSwitchControl.cpp \
    SPathTunnel.cpp \
    SBoardTunnel.cpp \
    SGraphicsTunnel.cpp \
    SBoardSwitch.cpp \
    SBoardStraight.cpp \
    SGameBoard.cpp \
    SBoardTurn.cpp \
    SException.cpp \
    SBoardTile.cpp \
    SBall.cpp \
    SPathTurn.cpp \
    SPathStraight.cpp \
    SPath.cpp \
    SGraphicsStraight.cpp \
    SGraphicsTurn.cpp \
    SConfig.cpp \
    SGraphicsRail.cpp \
    SGraphicsTile.cpp \
    main.cpp \
    SGraphicsItem.cpp \
    SGraphicsEdtSheet.cpp
FORMS += SConfigDlg.ui \
    SChgLayerSizeDlg.ui \
    SEdtLevelDlg.ui \
    SEdtLayerDlg.ui \
    SBoardGoalSettingsWidget.ui \
    SBoardTunnelSettingsWidget.ui \
    SOpenLevel.ui
RESOURCES += SerpTrain.qrc
