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
#ifndef UPDATENETWORKMANAGER_H
#define UPDATENETWORKMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QPointer>

#include "helper/singleton.h"

class UpdateNetworkManager : public QObject {
  Q_OBJECT
 private:
  explicit UpdateNetworkManager(QObject *parent = nullptr);

 public:
  SINGLETON(UpdateNetworkManager)
  ~UpdateNetworkManager() override;

  Q_INVOKABLE void get_latest_info();

  /**
   * @brief Initialize this class
   */
  void init();

 private:
  /**
   * @brief m_manager, which manage whole network access
   */
  QPointer<QNetworkAccessManager> m_manager;
 signals:
};

#endif  // UPDATENETWORKMANAGER_H
