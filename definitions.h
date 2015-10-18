#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QString>

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
