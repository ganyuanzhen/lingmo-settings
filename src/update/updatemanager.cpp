#include "updatemanager.h"

UpdateManager::UpdateManager(QObject* parent) : QObject{parent} {
  m_manager = std::make_shared<QNetworkAccessManager>(this);
}

UpdateManager::~UpdateManager() {}

void UpdateManager::check_for_update() {
  // 替换为真正的处理函数，待处理
  QString url = "http://127.0.0.1:4523/m1/4053566-0-default/api/update/latest";
  auto req = new QNetworkRequest(QUrl(url));
  connect(m_manager.get(), SIGNAL(finished(QNetworkReply*)), this,
          SLOT(handle_update_data(QNetworkReply*)));
  m_manager->get(*req);
}

bool UpdateManager::hasUpdate() { return has_updates_; }

void UpdateManager::handle_update_data(QNetworkReply* reply) {
  emit UpdateDataReply(reply->readAll());
  reply->deleteLater();
}
