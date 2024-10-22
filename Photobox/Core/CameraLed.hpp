#pragma once
#include <QObject>
#include <QtQmlIntegration>

namespace Pbox
{
class CameraLed : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("only enum")
  public:
    enum class Effect
    {
        Pulsate,
        Capture
    };
    Q_ENUM(Effect);

  public:
    Q_DISABLE_COPY_MOVE(CameraLed);
    CameraLed();
    ~CameraLed() override;

    Q_INVOKABLE virtual void playEffect(CameraLed::Effect effect) = 0;
    Q_INVOKABLE virtual void turnOff() = 0;
};
} // namespace Pbox
