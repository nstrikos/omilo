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
#define IndicAupMrFlite "Indic Aup Mr Flite"
#define IndicAxbGuFlite "Indic Axb Gu Flite"
#define IndicAxbHiFlite "Indic Axb Hi Flite"
#define IndicKnrTeFlite "Indic Knr Te Flite"
#define IndicSksTaFlite "Indic Sks Ta Flite"
#define IndicSlpMrFlite "Indic Slp Mr Flite"
#define IndicSxsHiFlite "Indic Sxs Hi Flite"
#define IndicSxvTaFlite "Indic Sxv Ta Flite"
#define AewFlite "Aew Flite"
#define AhwFlite "Ahw Flite"
#define AupFlite "Aup Flite"
#define Awb2Flite "Awb2 Flite"
#define AxbFlite "Axb Flite"
#define BdlFlite "Bdl Flite"
#define ClbFlite "Clb Flite"
#define EeyFlite "Eey Flite"
#define FemFlite "Fem Flite"
#define GkaFlite "Gka Flite"
#define JmkFlite "Jmk Flite"
#define KspFlite "Ksp Flite"
#define LjmFlite "Ljm Flite"
#define Rms2Flite "Rms2 Flite"
#define RxrFlite "Rxr Flite"
#define Slt2Flite "Slt2 Flite"
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

const QString wavFile = "/tmp/omilo.wav";
const QString textFile = "/tmp/omilo.txt";
const QString voiceFile = "/tmp/omiloVoice.txt";
const QString voiceTextEditorFile = "/tmp/omiloTextEditorVoice.txt";
const QString showCommandFile = "/tmp/omiloShowcommand.txt";
const QString silenceFile = "/tmp/omilo-silence.wav";
const unsigned int maxMaryMemory = 2048;
const unsigned int minMaryMemory = 350;

const QString defaultFestivalCommand = "/usr/bin/text2wave";
const QString defaultFestivalCommandArguments = "-eval '(voice_kal_diphone)'";

const QString tempPath = QStandardPaths::locate(QStandardPaths::TempLocation, QString(), QStandardPaths::LocateDirectory);
const QString testFile = tempPath + "omilo-test.wav";
const QString wavPrefix = tempPath + "omilo-";
const QString tempFile = tempPath + "omilo-tmp.wav";
const QString expPrefix = tempPath + "omilo-exp-";
const QString expTempFile = tempPath + "omilo-exp-tmp.wav";

#ifdef Q_OS_WIN
const QString maryHttpAddress = "http://localhost:59125/process?INPUT_TEXT=";
const QString soxCommand = QDir::currentPath() + "\\sox\\sox.exe";
const QString dummyCommand = soxCommand;
const QString maryInstallationDir = QDir::currentPath() + "\\marytts\\lib\\";
#define defaultVoice RmsFlite;

#else
const QString maryHttpAddress = "http://localhost:59127/process?INPUT_TEXT=";
const QString soxCommand = "sox ";
const QString dummyCommand = "echo foo";
const QString maryInstallationDir = "/usr/share/omilo-qt5/marytts-5.0";
#define defaultVoice KalFestival;
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
