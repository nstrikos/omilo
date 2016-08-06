#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QString>
#include <QStandardPaths>
#include <QDir>

#define KalFestival "Kal Festival"
#define AwbCmuFestival "Awb Cmu Festival"
#define BdlCmuFestival "Bdl Cmu Festival"
#define ClbCmuFestival "Clb Cmu Festival"
#define JmkCmuFestival "Jmk Cmu Festival"
#define KspCmuFestival "Ksp Cmu Festival"
#define RmsCmuFestival "Rms Cmu Festival"
#define SltCmuFestival "Slt Cmu Festival"
#define CustomFestival "Custom Festival"
#define RmsFlite "Rms Flite"
#define SltFlite "Slt Flite"
#define AwbFlite "Awb Flite"
#define Kal16Flite "Kal16 Flite"
#define FemFlite "Fem Flite"
#define JmkFlite "Jmk Flite"
#define RxrFlite "Rxr Flite"
#define ItalianMary "Mary Italian"
#define GermanMary "Mary German"
#define TurkishMary "Mary Turkish"
#define GreekMary "Mary Greek"
#define GreekHercules "Mary Hercules Greek"
#define GreekGoogleMary "Google Mary Greek"
#define FrenchMary "Mary French"
#define CmuBdlMary "Mary Cmu Bdl"
#define CmuRmsMary "Mary Cmu Rms"
#define CmuSltMary "Mary Cmu Slt"
#define ObadiahMary "Mary Obadiah"
#define PoppyMary "Mary Poppy"
#define PrudenceMary "Mary Prudence"
#define SpikeMary "Mary Spike"

#define defDurationStretch 100
#define defTargetMean 100
#define defSltTargetMean 150

const QString wavFile = "/tmp/omilo.wav";
const QString textFile = "/tmp/omilo.txt";
const QString voiceFile = "/tmp/omiloVoice.txt";
const QString voiceTextEditorFile = "/tmp/omiloTextEditorVoice.txt";
const QString showCommandFile = "/tmp/omiloShowcommand.txt";
const unsigned int maxMaryMemory = 2048;
const unsigned int minMaryMemory = 350;

const QString defaultFestivalCommand = "/usr/bin/text2wave";
const QString defaultFestivalCommandArguments = "-eval '(voice_kal_diphone)'";

const QString tempPath = QStandardPaths::locate(QStandardPaths::TempLocation, QString(), QStandardPaths::LocateDirectory);
const QString testFile = tempPath + "omilo-test.wav";
const QString wavPrefix = tempPath + "omilo-";

#ifdef Q_OS_WIN
const QString maryHttpAddress = "http://localhost:59125/process?INPUT_TEXT=";
#else
const QString maryHttpAddress = "http://localhost:59127/process?INPUT_TEXT=";
#endif

#ifdef Q_OS_WIN
const QString maryInstallationDir = QDir::currentPath() + "\\marytts\\lib\\";
;
#else
const QString maryInstallationDir = "/usr/share/omilo-qt5/marytts-5.0";
#endif


#define maximumNumberOfFiles 1000000

enum engineMode
{
    unknown = -1,
    festival = 0,
    festivalCmu = 1,
    flite = 2,
    marytts = 3
};

struct VoiceInfo
{
    engineMode mode;
    QString name;
    QString language;
};

class TextProcessItem
{
public:
    unsigned int id;
    QString text;
    unsigned int begin;
    unsigned int end;
    QString filename;
};

#endif // DEFINITIONS_H
