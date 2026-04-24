// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>

namespace Pbox
{
/**
 * @brief Interface for controlling camera LED effects.
 *
 * This class provides methods to play LED effects and turn off the LED.
 */
class CameraLed : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("only enum")
  public:
    /**
     * @brief Enumeration of LED effects.
     */
    enum class Effect
    {
        Pulsate, // Pulsating effect.
        Capture  // Capture effect.
    };
    Q_ENUM(Effect);

  public:
    Q_DISABLE_COPY_MOVE(CameraLed);
    /**
     * @brief Constructs a CameraLed.
     */
    CameraLed();
    /**
     * @brief Destructs the CameraLed.
     */
    ~CameraLed() override;

    /**
     * @brief Plays the specified LED effect.
     * @param effect The effect to play.
     */
    Q_INVOKABLE virtual void playEffect(CameraLed::Effect effect) = 0;
    /**
     * @brief Turns off the LED.
     */
    Q_INVOKABLE virtual void turnOff() = 0;
};
} // namespace Pbox
