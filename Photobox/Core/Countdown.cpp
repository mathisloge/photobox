// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Countdown.hpp"

namespace Pbox
{
Countdown::Countdown()
{
    timer_.setInterval(std::chrono::seconds{1});
    timer_.setTimerType(Qt::TimerType::PreciseTimer);
    timer_.setSingleShot(false);
    connect(&timer_, &QTimer::timeout, this, &Countdown::handleTimeout);
}

Countdown::~Countdown() = default;

void Countdown::start()
{
    setVisible(true);
    current_count_ = seconds_;
    updateText();
    timer_.start();
}

void Countdown::stop()
{
    setVisible(false);
    timer_.stop();
}

void Countdown::setSeconds(std::uint8_t seconds)
{
    seconds_ = seconds;
}

const QString &Countdown::getText() const
{
    return text_;
}

bool Countdown::isVisible() const
{
    return visible_;
}

void Countdown::updateText()
{
    QString new_text{QString::number(current_count_)};
    if (current_count_ <= 1)
    {
        new_text = QStringLiteral("Smile!");
    }

    if (new_text != text_)
    {
        text_ = std::move(new_text);
        Q_EMIT textChanged();
    }
}

void Countdown::handleTimeout()
{
    current_count_--;
    if (current_count_ > 0)
    {
        updateText();
    }
    else
    {
        stop();
        Q_EMIT finished();
    }
    Q_EMIT currentCountChanged(current_count_);
}

void Countdown::setVisible(bool visible)
{
    if (visible_ != visible)
    {
        visible_ = visible;
        Q_EMIT visibleChanged();
    }
}

} // namespace Pbox
