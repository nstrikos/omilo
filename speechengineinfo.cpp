#include "speechengineinfo.h"

SpeechEngineInfo::SpeechEngineInfo()
{
    update();
}

//If we need to add a new voice we need to re-code this function
void SpeechEngineInfo::update()
{
    installedVoices.clear();
    availableVoices.clear();
    VoiceInfo voiceToAdd;

    voiceToAdd.mode = festival;
    voiceToAdd.name = KalFestival;
    voiceToAdd.language = QLocale::languageToString(QLocale::English);
    if(QDir("/usr/share/omilo-qt5/festival/lib/voices/english/kal_diphone").exists())
       installedVoices.push_back(voiceToAdd);
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.mode = festivalCmu;
    voiceToAdd.name = AwbCmuFestival;
    voiceToAdd.language = QLocale::languageToString(QLocale::English);
    if(QDir("/usr/share/festival/voices/english/cmu_us_awb_arctic_clunits").exists())
        installedVoices.push_back(voiceToAdd);
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = BdlCmuFestival;
    voiceToAdd.language = QLocale::languageToString(QLocale::English);
    if(QDir("/usr/share/festival/voices/english/cmu_us_bdl_arctic_clunits").exists())
        installedVoices.push_back(voiceToAdd);
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = ClbCmuFestival;
    voiceToAdd.language = QLocale::languageToString(QLocale::English);
    if(QDir("/usr/share/festival/voices/english/cmu_us_clb_arctic_clunits").exists())
        installedVoices.push_back(voiceToAdd);
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = JmkCmuFestival;
    voiceToAdd.language = QLocale::languageToString(QLocale::English);
    if(QDir("/usr/share/festival/voices/english/cmu_us_jmk_arctic_clunits").exists())
        installedVoices.push_back(voiceToAdd);
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = KspCmuFestival;
    voiceToAdd.language = QLocale::languageToString(QLocale::English);
    if(QDir("/usr/share/festival/voices/english/cmu_us_ksp_arctic_clunits").exists())
        installedVoices.push_back(voiceToAdd);
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = RmsCmuFestival;
    voiceToAdd.language = QLocale::languageToString(QLocale::English);
    if(QDir("/usr/share/festival/voices/english/cmu_us_rms_arctic_clunits").exists())
        installedVoices.push_back(voiceToAdd);
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = SltCmuFestival;
    voiceToAdd.language = QLocale::languageToString(QLocale::English);
    if(QDir("/usr/share/festival/voices/english/cmu_us_slt_arctic_clunits").exists())
        installedVoices.push_back(voiceToAdd);
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.mode = flite;
    voiceToAdd.language = QLocale::languageToString(QLocale::English);
    if(QFile::exists("/usr/bin/flite"))
    {
        voiceToAdd.name = RmsFlite;
        installedVoices.push_back(voiceToAdd);
        voiceToAdd.name = SltFlite;
        installedVoices.push_back(voiceToAdd);
        voiceToAdd.name = AwbFlite;
        installedVoices.push_back(voiceToAdd);
        voiceToAdd.name = Kal16Flite;
        installedVoices.push_back(voiceToAdd);
    }
    else
    {
        voiceToAdd.name = RmsFlite;
        availableVoices.push_back(voiceToAdd);
        voiceToAdd.name = SltFlite;
        availableVoices.push_back(voiceToAdd);
        voiceToAdd.name = AwbFlite;
        availableVoices.push_back(voiceToAdd);
        voiceToAdd.name = Kal16Flite;
        availableVoices.push_back(voiceToAdd);
    }

    voiceToAdd.mode = marytts;
    if (QDir("/usr/share/omilo-qt5/marytts-5.0").exists())
    {
        voiceToAdd.name = ItalianMary;
        voiceToAdd.language = QLocale::languageToString(QLocale::Italian);
        installedVoices.push_back(voiceToAdd);
        voiceToAdd.name = GermanMary;
        voiceToAdd.language = QLocale::languageToString(QLocale::German);
        installedVoices.push_back(voiceToAdd);
        voiceToAdd.name = TurkishMary;
        voiceToAdd.language = QLocale::languageToString(QLocale::Turkish);
        installedVoices.push_back(voiceToAdd);
        voiceToAdd.name = GreekMary;
        voiceToAdd.language = QLocale::languageToString(QLocale::Greek);
        installedVoices.push_back(voiceToAdd);
        voiceToAdd.name = GreekGoogleMary;
        voiceToAdd.language = QLocale::languageToString(QLocale::Greek);
        installedVoices.push_back(voiceToAdd);
        voiceToAdd.name = FrenchMary;
        voiceToAdd.language = QLocale::languageToString(QLocale::French);
        installedVoices.push_back(voiceToAdd);
        voiceToAdd.language = QLocale::languageToString(QLocale::English);
        voiceToAdd.name = CmuRmsMary;
        installedVoices.push_back(voiceToAdd);
        voiceToAdd.name = CmuSltMary;
        installedVoices.push_back(voiceToAdd);
        voiceToAdd.name = ObadiahMary;
        installedVoices.push_back(voiceToAdd);
        voiceToAdd.name = PoppyMary;
        installedVoices.push_back(voiceToAdd);
        voiceToAdd.name = PrudenceMary;
        installedVoices.push_back(voiceToAdd);
        voiceToAdd.name = SpikeMary;
        installedVoices.push_back(voiceToAdd);
    }
    else
    {
        voiceToAdd.name = ItalianMary;
        voiceToAdd.language = QLocale::languageToString(QLocale::Italian);
        availableVoices.push_back(voiceToAdd);
        voiceToAdd.name = GermanMary;
        voiceToAdd.language = QLocale::languageToString(QLocale::German);
        availableVoices.push_back(voiceToAdd);
        voiceToAdd.name = TurkishMary;
        voiceToAdd.language = QLocale::languageToString(QLocale::Turkish);
        availableVoices.push_back(voiceToAdd);
        voiceToAdd.name = GreekMary;
        voiceToAdd.language = QLocale::languageToString(QLocale::Greek);
        availableVoices.push_back(voiceToAdd);
        voiceToAdd.name = FrenchMary;
        voiceToAdd.language = QLocale::languageToString(QLocale::French);
        availableVoices.push_back(voiceToAdd);
        voiceToAdd.language = QLocale::languageToString(QLocale::English);
        voiceToAdd.name = CmuRmsMary;
        availableVoices.push_back(voiceToAdd);
        voiceToAdd.name = CmuSltMary;
        availableVoices.push_back(voiceToAdd);
        voiceToAdd.name = ObadiahMary;
        availableVoices.push_back(voiceToAdd);
        voiceToAdd.name = PoppyMary;
        availableVoices.push_back(voiceToAdd);
        voiceToAdd.name = PrudenceMary;
        availableVoices.push_back(voiceToAdd);
        voiceToAdd.name = SpikeMary;
        availableVoices.push_back(voiceToAdd);
    }
}
