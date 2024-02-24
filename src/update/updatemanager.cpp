/*
 * Copyright (C) 2024 LingmoOS Team.
 *
 * Author:     Elysia <c.elysia@foxmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "updatemanager.h"

#include <QSysInfo>
#include <QtConcurrent>

#include "server_config.h"

UpdateManager::UpdateManager(QObject* parent) : QObject{parent} {
  m_manager = std::make_shared<QNetworkAccessManager>();
  setDownloadWidget(new DownloadListWidget);
}

UpdateManager::~UpdateManager() { dw->deleteLater(); }

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
  if (reply->url() ==
      QUrl("http://127.0.0.1:4523/m1/4053566-0-default/api/update/latest")) {
    emit updateDataReply(reply->readAll());
  }
  reply->deleteLater();
}

void UpdateManager::startCheckforUpdate() { this->check_for_update(); }

void UpdateManager::requestDownloadandInstall(QString name,
                                              QString package_name, QString url,
                                              int index) {
  qDebug() << name << " " << package_name << " " << index << " " << url;

  // 需要按实际修改
  QString downloadUrl = QString(DEFAULTURL) +
                        QSysInfo::currentCpuArchitecture() + "/distroversion" +
                        package_name;

  // addItem 后自动下载安装
  DownloadItem* item =
      dw->addItem(name, package_name + ".deb", package_name, url, index);

  if (item == nullptr) {
    return;
  }

  connect(item, SIGNAL(finished(int, int)), this,
          SLOT(handleDownloadResult(int, int)));

  emit addedToProcessingQueue(index);
}

void UpdateManager::setDownloadWidget(DownloadListWidget* dw_) {
  if (dw) {
    dw->deleteLater();
    dw = nullptr;
  }

  dw = dw_;
};

void UpdateManager::handleDownloadResult(int status, int index) {}
