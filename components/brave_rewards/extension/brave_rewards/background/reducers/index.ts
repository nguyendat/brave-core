/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import { combineReducers } from 'redux'

// Utils
import { rewardsPanelReducer } from './rewards_panel_reducer'
import { ApplicationState } from '../../constants/rewardsPanelState'

export default combineReducers<ApplicationState>({
  rewardsPanelData: rewardsPanelReducer
})