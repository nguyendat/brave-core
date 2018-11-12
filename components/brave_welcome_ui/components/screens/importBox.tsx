/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

// Feature-specific components
import { Content, Title, ImportImage, Paragraph } from 'brave-ui/features/welcome'

// Shared components
import { Button } from 'brave-ui/components'

// Utils
import { getLocale } from '../../../common/locale'

// Images
const importImage = require('../../../img/welcome/welcome_import.svg')

interface Props {
  index: number
  currentScreen: number
  onClick: () => void
}

export default class ImportBox extends React.PureComponent<Props, {}> {
  render () {
    const { index, currentScreen, onClick } = this.props
    return (
      <Content
        zIndex={index}
        active={index === currentScreen}
        screenPosition={'1' + (index + 1) + '0%'}
        isPrevious={index <= currentScreen}
      >
        <ImportImage src={importImage} />
        <Title>{getLocale('importFromAnotherBrowser')}</Title>
        <Paragraph>{getLocale('setupImport')}</Paragraph>
          <Button
            level='primary'
            type='accent'
            size='large'
            text={getLocale('import')}
            onClick={onClick}
          />
      </Content>
    )
  }
}
