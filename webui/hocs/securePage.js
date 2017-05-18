import React from 'react';
import PropTypes from 'prop-types';

import defaultPage from './defaultPage'

const securePageHoc = Page => class SecurePage extends React.Component {
  static getInitialProps (ctx) {
    return Page.getInitialProps && Page.getInitialProps(ctx)
  }
  static propTypes = {
    isAuthenticated: PropTypes.bool.isRequired
  }
  render () {
    if (!this.props.isAuthenticated) {
      return (<p>Unauthroized</p>);
    }
    return (<Page {...this.props} />);
  }
}

export default Page => defaultPage(securePageHoc(Page))
