import React, { PropTypes } from 'react'

import SecurePage from '../hocs/securePage'

const Secret = ({ Username, Role }) => (
  <div>
      Hi <strong>{Username}:{Role}</strong>. 
      This is a super secure page! 
      Try loading this page again using the incognito/private mode of your browser.
  </div>
)

Secret.propTypes = {
  Username: PropTypes.string.isRequired
}

export default SecurePage(Secret)
