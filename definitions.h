#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QString>
#include <QLocale>
#include <limits>

#define KalFestival "Kal Festival"
#define AwbCmuFestival "Awb Cmu Festival"
#define BdlCmuFestival "Bdl Cmu Festival"
#define ClbCmuFestival "Clb Cmu Festival"
#define JmkCmuFestival "Jmk Cmu Festival"
#define KspCmuFestival "Ksp Cmu Festival"
#define RmsCmuFestival "Rms Cmu Festival"
#define SltCmuFestival "Slt Cmu Festival"
#define RmsFlite "Rms Flite"
#define SltFlite "Slt Flite"
#define AwbFlite "Awb Flite"
#define Kal16Flite "Kal16 Flite"
#define ItalianMary "Mary Italian"
#define GermanMary "Mary German"
#define TurkishMary "Mary Turkish"
#define GreekMary "Mary Greek"
#define GreekGoogleMary "Google Mary Greek"
#define FrenchMary "Mary French"
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

#define maximumNumberOfFiles 1000000

enum engineMode
{
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

#endif // DEFINITIONS_H
