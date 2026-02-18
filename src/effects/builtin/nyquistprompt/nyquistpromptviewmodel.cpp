/*
 * Audacity: A Digital Audio Editor
 */
#include "nyquistpromptviewmodel.h"
#include "nyquistprompteffect.h"

#include "au3wrap/internal/wxtypes_convert.h"
#include "framework/global/log.h"
#include "framework/global/translation.h"

#include "effects/nyquist/internal/nyquistparameterextractorservice.h"

#include <QFileDialog>
#include <QDir>
#include <QFileInfo>

using namespace au::effects;
using namespace muse;

NyquistPromptViewModel::NyquistPromptViewModel(QObject* parent, int instanceId)
    : BuiltinEffectModel(parent, instanceId)
{
}

QString NyquistPromptViewModel::commandText() const
{
    return m_commandText;
}

void NyquistPromptViewModel::setCommandText(const QString& text)
{
    if (m_commandText == text) {
        return;
    }

    effect<NyquistPromptEffect>().commandText() = au3::wxFromString(text);

    m_commandText = text;
    emit commandTextChanged();
}

QString NyquistPromptViewModel::title() const
{
    return QObject::tr("Nyquist prompt");
}

void NyquistPromptViewModel::loadScript()
{
    const QString filter = QObject::tr("Nyquist scripts") + " (*.ny);;"
                           + QObject::tr("Lisp scripts") + " (*.lsp);;"
                           + QObject::tr("All files") + " (*)";

    const QString filePath = QFileDialog::getOpenFileName(
        nullptr,
        QObject::tr("Load Nyquist script"),
        m_lastFilePath.isEmpty() ? QDir::homePath() : QFileInfo(m_lastFilePath).absolutePath(),
        filter
        );

    if (filePath.isEmpty()) {
        return;
    }

    // Load the file (overriding current text)
    const io::path_t path = io::path_t(filePath.toStdString());
    Ret ret = fileSystem()->exists(path);
    if (!ret) {
        LOGE() << "File does not exist: " << filePath;
        return;
    }

    ByteArray data;
    ret = fileSystem()->readFile(path, data);
    if (!ret) {
        LOGE() << "Failed to read file: " << filePath;
        return;
    }

    m_lastFilePath = filePath;

    // Find the actual end of the text (before any null bytes)
    size_t actualSize = data.size();
    const char* dataPtr = data.constChar();
    for (size_t i = 0; i < data.size(); ++i) {
        if (dataPtr[i] == '\0') {
            actualSize = i;
            break;
        }
    }

    // Convert only the actual text content, excluding null padding
    const QString loadedText = QString::fromUtf8(dataPtr, static_cast<int>(actualSize));
    setCommandText(loadedText);
}

void NyquistPromptViewModel::saveScript()
{
    const QString filter = QObject::tr("Nyquist scripts") + " (*.ny);;"
                           + QObject::tr("Lisp scripts") + " (*.lsp);;"
                           + QObject::tr("All files") + " (*)";

    const QString filePath = QFileDialog::getSaveFileName(
        nullptr,
        QObject::tr("Save Nyquist script"),
        m_lastFilePath.isEmpty() ? QDir::homePath() : m_lastFilePath,
        filter
        );

    if (filePath.isEmpty()) {
        return;
    }

    // Save the file
    const io::path_t path = io::path_t(filePath.toStdString());
    const QByteArray qdata = m_commandText.toUtf8();
    const ByteArray data = ByteArray::fromQByteArrayNoCopy(qdata);

    const Ret ret = fileSystem()->writeFile(path, data);
    if (!ret) {
        LOGE() << "Failed to write file: " << filePath;
        return;
    }

    m_lastFilePath = filePath;
}

void NyquistPromptViewModel::debugEffect()
{
    // Get the Nyquist parameter extractor
    NyquistParameterExtractorService* extractor = getExtractor();
    if (!extractor) {
        return;
    }

    EffectInstance* instance = instancesRegister()->instanceById(m_instanceId).get();
    if (!instance) {
        return;
    }

    // Get the settings access
    EffectSettingsAccessPtr settingsAccess = instancesRegister()->settingsAccessById(m_instanceId);
    if (!settingsAccess) {
        return;
    }

    // Execute the Nyquist code for debugging (without starting audio playback)
    String debugOutput;
    settingsAccess->ModifySettings([extractor, instance, &debugOutput](EffectSettings& settings) {
        // Execute the effect and capture debug output
        debugOutput = extractor->executeForDebug(instance, settings);
        return nullptr;
    });

    // Show the debug output in a dialog
    if (!debugOutput.isEmpty()) {
        interactive()->info(
            muse::trc("effects", "Nyquist Debug Output"),
            debugOutput.toStdString()
            );
    } else {
        interactive()->info(
            muse::trc("effects", "Nyquist Debug Output"),
            muse::mtrc("effects", "No debug output generated.").toStdString()
            );
    }
}

void NyquistPromptViewModel::doReload()
{
    // Load the current command text from the effect instance
    m_commandText = QString::fromStdString(au3::wxToStdString(effect<NyquistPromptEffect>().commandText()));
    emit commandTextChanged();
}

NyquistParameterExtractorService* NyquistPromptViewModel::getExtractor() const
{
    // Get the Nyquist parameter extractor
    // We know this is a Nyquist effect, so we can safely cast
    return dynamic_cast<NyquistParameterExtractorService*>(
        parameterExtractorRegistry()
        ? parameterExtractorRegistry()->extractorForFamily(EffectFamily::Nyquist)
        : nullptr
        );
}
