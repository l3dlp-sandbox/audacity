/*
* Audacity: A Digital Audio Editor
*/

#include <chrono>

#include <QCoreApplication>
#include <QEventLoop>

#include "progressdialog.h"
#include "wxtypes_convert.h"

ProgressDialog::ProgressDialog(const muse::modularity::ContextPtr& ctx, const std::string& title)
    : muse::Contextable(ctx), m_progressTitle{title}
{
    // Of course, the least number of increments to yield a smooth animation depends on the width of the progress bar,
    // yet 300 increments should be enough to provide a smooth animation in most cases.
    m_progress.setMaxNumIncrements(200);
}

ProgressDialog::ProgressDialog(const muse::modularity::ContextPtr& ctx, const TranslatableString& title)
    : ProgressDialog{ctx, au::au3::wxToStdString(title.Translation())}
{
}

ProgressDialog::~ProgressDialog()
{
    m_progress.finish(muse::make_ok());
}

void ProgressDialog::Reinit()
{
}

void ProgressDialog::SetDialogTitle(const TranslatableString& title)
{
    m_progressTitle = au::au3::wxToStdString(title.Translation());
}

ProgressResult ProgressDialog::Poll(unsigned long long numerator, unsigned long long denominator, const TranslatableString& message)
{
    if (m_cancelled) {
        return ProgressResult::Cancelled;
    }

    if (!m_progress.isStarted()) {
        interactive()->showProgress(m_progressTitle, m_progress);

        if (!m_canceledHooked) {
            m_progress.canceled().onNotify(this, [this]() {
                m_cancelled = true;
            });
            m_canceledHooked = true;
        }

        m_progress.start();
    }

    if (!message.empty()) {
        m_progressMessage = au::au3::wxToStdString(message.Translation());
    }

    // Push the new fraction/message into muse::Progress unconditionally.
    // The framework throttles the *visual* update internally;
    m_progress.progress(numerator, denominator, m_progressMessage);

    // muse::Progress's internal throttle only fires when the numerator/
    // denominator pair has advanced — callers inside a recursive walk can
    // call Poll() many times with the *same* (numerator, denominator) and
    // only the message changing. In that window the QML side
    // never wakes up, the GUI thread starves, and macOS shows the
    // beachball cursor. Pump the event loop on an independent time-based
    // throttle so the dialog stays responsive even when the bar is idle.
    using clock = std::chrono::steady_clock;
    constexpr auto pumpInterval = std::chrono::milliseconds(50);
    const auto now = clock::now();
    if (now - m_lastEventPump >= pumpInterval) {
        m_lastEventPump = now;
        // Give the GUI thread a short time budget inside the event loop so
        // the render thread can grab a sync window and actually paint the
        // updated dialog. On macOS with Qt's threaded render loop, calling
        // `processEvents()` with no budget returns immediately and the
        // pending QML sync never lands until something else (e.g. window
        // focus change) wakes the loop.
        QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }

    return ProgressResult::Success;
}

void ProgressDialog::SetMessage(const TranslatableString& message)
{
    m_progressMessage = au::au3::wxToStdString(message.Translation());
}
