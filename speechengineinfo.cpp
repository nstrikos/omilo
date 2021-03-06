#include "speechengineinfo.h"

SpeechEngineInfo::SpeechEngineInfo()
{
    qDebug() << "Creating new information class...";
    update();
    qDebug() << "Creating new information class completed.";
}

SpeechEngineInfo::~SpeechEngineInfo()
{
    qDebug() << "Deleting information class...";
}

//If we need to add a new voice we need to re-code this function
void SpeechEngineInfo::update()
{
    qDebug() << "Updating speech engine info...";
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
    if(QDir("/usr/share/omilo-qt5/festival/lib/voices/english/cmu_us_awb_arctic_clunits").exists())
        installedVoices.push_back(voiceToAdd);
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = BdlCmuFestival;
    voiceToAdd.language = QLocale::languageToString(QLocale::English);
    if(QDir("/usr/share/omilo-qt5/festival/lib/voices/english/cmu_us_bdl_arctic_clunits").exists())
        installedVoices.push_back(voiceToAdd);
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = ClbCmuFestival;
    voiceToAdd.language = QLocale::languageToString(QLocale::English);
    if(QDir("/usr/share/omilo-qt5/festival/lib/voices/english/cmu_us_clb_arctic_clunits").exists())
        installedVoices.push_back(voiceToAdd);
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = JmkCmuFestival;
    voiceToAdd.language = QLocale::languageToString(QLocale::English);
    if(QDir("/usr/share/omilo-qt5/festival/lib/voices/english/cmu_us_jmk_arctic_clunits").exists())
        installedVoices.push_back(voiceToAdd);
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = KspCmuFestival;
    voiceToAdd.language = QLocale::languageToString(QLocale::English);
    if(QDir("/usr/share/omilo-qt5/festival/lib/voices/english/cmu_us_ksp_arctic_clunits").exists())
        installedVoices.push_back(voiceToAdd);
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = RmsCmuFestival;
    voiceToAdd.language = QLocale::languageToString(QLocale::English);
    if(QDir("/usr/share/omilo-qt5/festival/lib/voices/english/cmu_us_rms_arctic_clunits").exists())
        installedVoices.push_back(voiceToAdd);
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = SltCmuFestival;
    voiceToAdd.language = QLocale::languageToString(QLocale::English);
    if(QDir("/usr/share/omilo-qt5/festival/lib/voices/english/cmu_us_slt_arctic_clunits").exists())
        installedVoices.push_back(voiceToAdd);
    else
        availableVoices.push_back(voiceToAdd);

 #ifndef Q_OS_WIN
    voiceToAdd.name = CustomFestival;
    installedVoices.push_back(voiceToAdd);
#endif

    voiceToAdd.mode = flite;
    voiceToAdd.language = QLocale::languageToString(QLocale::English);
    if (QFile::exists("/usr/share/omilo-qt5/flite/flite") ||
        QFile::exists(QCoreApplication::applicationDirPath() + "\\flite.exe") )
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

    voiceToAdd.name = IndicAupMrFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_indic_aup_mr.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = IndicAxbGuFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_indic_axb_gu.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = IndicAxbHiFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_indic_axb_hi.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = IndicKnrTeFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_indic_knr_te.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = IndicSksTaFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_indic_sks_ta.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = IndicSlpMrFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_indic_slp_mr.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = IndicSxsHiFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_indic_sxs_hi.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = IndicSxvTaFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_indic_sxv_ta.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = AewFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_us_aew.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = AhwFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_us_ahw.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = AupFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_us_aup.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = Awb2Flite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_us_awb.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = AxbFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_us_axb.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = BdlFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_us_bdl.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = ClbFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_us_clb.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = EeyFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_us_eey.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = FemFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_us_fem.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = GkaFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_us_gka.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = JmkFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_us_jmk.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = KspFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_us_ksp.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = LjmFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_us_ljm.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = Rms2Flite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_us_rms.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = RxrFlite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_us_rxr.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.name = Slt2Flite;
    if (QFile::exists("/usr/share/omilo-qt5/flite/voices/cmu_us_slt.flitevox"))
    {
        installedVoices.push_back(voiceToAdd);
    }
    else
        availableVoices.push_back(voiceToAdd);

    voiceToAdd.mode = marytts;
    if (QDir(maryInstallationDir).exists())
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
        voiceToAdd.name = GreekHercules;
        voiceToAdd.language = QLocale::languageToString(QLocale::Greek);
        installedVoices.push_back(voiceToAdd);
        voiceToAdd.name = GreekGoogleMary;
        voiceToAdd.language = QLocale::languageToString(QLocale::Greek);
        installedVoices.push_back(voiceToAdd);
        voiceToAdd.name = FrenchMary;
        voiceToAdd.language = QLocale::languageToString(QLocale::French);
        installedVoices.push_back(voiceToAdd);
        voiceToAdd.language = QLocale::languageToString(QLocale::English);
        voiceToAdd.name = CmuBdlMary;
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
        voiceToAdd.name = CoriMary;
        installedVoices.push_back(voiceToAdd);
    }
}

engineMode SpeechEngineInfo::getVoiceMode(QString voice)
{
    unsigned int i;
    QString voiceString;

    if (installedVoices.size() > 0)
    {
        for (i = 0; i < installedVoices.size() - 1; i++)
        {
            voiceString = installedVoices.at(i).name;
            if (voiceString == voice)
            {
                return installedVoices.at(i).mode;
            }
        }
    }
    return unknown;
}
