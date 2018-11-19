/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/importer/brave_profile_writer.h"
#include "brave/common/importer/brave_stats.h"
#include "brave/common/importer/brave_referral.h"
#include "brave/common/pref_names.h"
#include "brave/utility/importer/brave_importer.h"

#include "base/time/time.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/storage_partition.h"
#include "components/prefs/pref_service.h"
#include "net/cookies/canonical_cookie.h"
#include "net/cookies/cookie_constants.h"
#include "net/url_request/url_request_context.h"
#include "net/url_request/url_request_context_getter.h"
#include "services/network/public/mojom/cookie_manager.mojom.h"

BraveProfileWriter::BraveProfileWriter(Profile* profile)
    : ProfileWriter(profile) {}

BraveProfileWriter::~BraveProfileWriter() {}

void BraveProfileWriter::AddCookies(
    const std::vector<net::CanonicalCookie>& cookies) {
  network::mojom::CookieManagerPtr cookie_manager;
  content::BrowserContext::GetDefaultStoragePartition(profile_)
      ->GetNetworkContext()
      ->GetCookieManager(mojo::MakeRequest(&cookie_manager));

  for (auto& cookie : cookies) {
    cookie_manager->SetCanonicalCookie(
        cookie,
        true,  // secure_source
        true,  // modify_http_only
        // Fire and forget
        network::mojom::CookieManager::SetCanonicalCookieCallback());
  }
}

void BraveProfileWriter::UpdateStats(const BraveStats& stats) {
  PrefService* prefs = profile_->GetOriginalProfile()->GetPrefs();

  const uint64_t ads_blocked = prefs->GetUint64(kAdsBlocked);
  const uint64_t trackers_blocked = prefs->GetUint64(kTrackersBlocked);
  const uint64_t https_upgrades = prefs->GetUint64(kHttpsUpgrades);

  // Only update the current stats if they are less than the imported
  // stats; intended to prevent incorrectly updating the stats multiple
  // times from multiple imports.
  if (ads_blocked < uint64_t{stats.adblock_count}) {
      prefs->SetUint64(kAdsBlocked, ads_blocked + stats.adblock_count);
  }
  if (trackers_blocked < uint64_t{stats.trackingProtection_count}) {
    prefs->SetUint64(kTrackersBlocked,
                     trackers_blocked + stats.trackingProtection_count);
  }
  if (https_upgrades < uint64_t{stats.httpsEverywhere_count}) {
    prefs->SetUint64(kHttpsUpgrades,
                     https_upgrades + stats.httpsEverywhere_count);
  }
}

void BraveProfileWriter::UpdateReferral(const BraveReferral& referral) {
  LOG(ERROR) << "BSC]] 3"
    << "\nreferral.promo_code=" << referral.promo_code
    << "\nreferral.download_id=" << referral.download_id
    << "\nreferral.finalize_timestamp=" << referral.finalize_timestamp
    << "\nreferral.week_of_installation=" << referral.week_of_installation;

  PrefService* prefs = profile_->GetOriginalProfile()->GetPrefs();

  if (!referral.week_of_installation.empty()) {
    LOG(INFO) << "Setting kWeekOfInstallation to \"" << referral.week_of_installation << "\"";
    prefs->SetString(kWeekOfInstallation, referral.week_of_installation);
  }

  return;
  //TODO: fixme
  PrefService* local_state = NULL; //g_browser_process->local_state();

  if (!referral.promo_code.empty()) {
    LOG(INFO) << "Setting kReferralPromoCode to \"" << referral.promo_code << "\"";
    local_state->SetString(kReferralPromoCode, referral.promo_code);
  } else {
    local_state->ClearPref(kReferralPromoCode);
  }

  if (!referral.download_id.empty()) {
    LOG(INFO) << "Setting kReferralDownloadID to \"" << referral.download_id << "\"";
    local_state->SetString(kReferralDownloadID, referral.download_id);
  } else {
    local_state->ClearPref(kReferralDownloadID);
  }

  if(referral.finalize_timestamp > 0) {
    LOG(INFO) << "Setting kReferralTimestamp to \"" << referral.finalize_timestamp << "\"";
    local_state->SetTime(kReferralTimestamp, base::Time::FromJsTime(referral.finalize_timestamp));
  } else {
    local_state->ClearPref(kReferralTimestamp);
  }
}
