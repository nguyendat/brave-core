/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_ADS_ADS_SERVICE_IMPL_
#define BRAVE_COMPONENTS_BRAVE_ADS_ADS_SERVICE_IMPL_

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "bat/ads/ads_client.h"
#include "brave/components/brave_ads/browser/ads_service.h"

class Profile;

namespace ads {
class Ads;
}

namespace base {
class OneShotTimer;
class SequencedTaskRunner;
}  // namespace base

namespace brave_ads {

class BundleStateDatabase;

class AdsServiceImpl : public AdsService,
                       public ads::AdsClient,
                       public base::SupportsWeakPtr<AdsServiceImpl> {
 public:
  explicit AdsServiceImpl(Profile* profile);
  ~AdsServiceImpl() override;

  bool is_enabled() const override;

 private:
  void Init();


  // AdsClient implementation
  bool IsAdsEnabled() const override;
  const std::string GetAdsLocale() const override;
  uint64_t GetAdsPerHour() const override;
  uint64_t GetAdsPerDay() const override;
  const ads::ClientInfo GetClientInfo() const override;
  const std::vector<std::string> GetLocales() const override;
  const std::string GenerateUUID() const override;
  const std::string GetSSID() const override;
  void ShowNotification(
      const std::unique_ptr<ads::NotificationInfo> info) override {}
  uint32_t SetTimer(const uint64_t& time_offset) override;
  void KillTimer(uint32_t timer_id) override;
  std::unique_ptr<ads::URLSession> URLSessionTask(
      const std::string& url,
      const std::vector<std::string>& headers,
      const std::string& content,
      const std::string& content_type,
      const ads::URLSession::Method& method,
      ads::URLSessionCallbackHandlerCallback callback) override;
  void Save(const std::string& name,
            const std::string& value,
            ads::OnSaveCallback callback) override;
  void Load(const std::string& name,
            ads::OnLoadCallback callback) override;
  void SaveBundleState(
      std::unique_ptr<ads::BUNDLE_STATE> bundle_state,
      ads::OnSaveCallback callback) override;
  const std::string Load(const std::string& name) override;
  void Reset(const std::string& name,
             ads::OnResetCallback callback) override;
  void GetAdsForCategory(
      const std::string& category,
      ads::OnGetAdsForCategoryCallback callback) override;
  void GetAdsForSampleCategory(
      ads::OnGetAdsForCategoryCallback callback) override;
  bool GetUrlComponents(
      const std::string& url,
      ads::UrlComponents* components) const override;
  void EventLog(const std::string& json) override {}
  std::ostream& Log(const char* file,
                    int line,
                    const ads::LogLevel log_level) const override;

  void OnGetAdsForCategory(const ads::OnGetAdsForCategoryCallback& callback,
                     const std::string& category,
                     const std::vector<ads::AdInfo>& ads);
  void OnSaveBundleState(const ads::OnSaveCallback& callback, bool success);
  void OnLoaded(const ads::OnLoadCallback& callback,
                const std::string& value);
  void OnTimer(uint32_t timer_id);


  Profile* profile_;  // NOT OWNED
  const scoped_refptr<base::SequencedTaskRunner> file_task_runner_;
  const base::FilePath base_path_;

  std::map<uint32_t, std::unique_ptr<base::OneShotTimer>> timers_;
  uint32_t next_timer_id_;
  std::unique_ptr<BundleStateDatabase> bundle_state_backend_;

  std::unique_ptr<ads::Ads> ads_;

  DISALLOW_COPY_AND_ASSIGN(AdsServiceImpl);
};

}  // namespace brave_ads

#endif  // BRAVE_COMPONENTS_BRAVE_ADS_ADS_SERVICE_IMPL_