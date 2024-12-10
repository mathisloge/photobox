// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <QTimer>
#include <QtQmlIntegration>
#include <Pbox/DisableCopyMove.hpp>

namespace Pbox
{
class Countdown : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
    PBOX_DISABLE_COPY_MOVE(Countdown);

    Q_PROPERTY(bool visible READ isVisible NOTIFY visibleChanged);
    Q_PROPERTY(QString text READ getText NOTIFY textChanged);

  public:
    Countdown();
    ~Countdown() override;

    //! @brief starts the countdown
    void start();

    //! @brief stops the countdown
    //! @attention won't emit the finished signal.
    void stop();

    //! @brief the seconds the countdown should run
    //! @attention doesn't change the current running countdown
    void setSeconds(std::uint8_t seconds);

    /// vvv property methods
    const QString &getText() const;
    bool isVisible() const;
    /// ^^^ property methods

  Q_SIGNALS:
    //! @brief Will be emitted if the seconds have expired.
    void finished();

    /// vvv property signals
    void textChanged();
    void visibleChanged();
    void currentCountChanged(int count);
    /// ^^^ property signals

  private:
    void setVisible(bool visible);
    void updateText();
    void handleTimeout();

  private:
    QTimer timer_;
    bool visible_{false};
    QString text_;
    std::uint8_t seconds_{5};
    std::uint8_t current_count_;
};
} // namespace Pbox