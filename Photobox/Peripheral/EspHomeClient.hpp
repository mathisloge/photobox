#pragma once
#include <QNetworkAccessManager>
#include "IEspHomeClient.hpp"

namespace Pbox
{
class EspHomeClient : public IEspHomeClient
{
  public:
    Q_DISABLE_COPY_MOVE(EspHomeClient);
    explicit EspHomeClient(QUrl base_url);
    ~EspHomeClient() override;

    void post(std::string_view url_request) override;

  private:
    void subscribeEvents();
    void readEventReply(QNetworkReply &reply);

  private:
    QUrl base_url_;
    QNetworkAccessManager net_manager_;
};
} // namespace Pbox
