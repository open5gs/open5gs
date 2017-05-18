import React, { PropTypes } from 'react'

import securePage from '../hocs/securePage'

const Secret = ({ Username }) => (
  <div>
      Hi <strong>{Username}</strong>. 
      This is a super secure page! 
      Try loading this page again using the incognito/private mode of your browser.
  </div>
)

Secret.propTypes = {
  Username: PropTypes.string.isRequired
}

export default securePage(Secret)
