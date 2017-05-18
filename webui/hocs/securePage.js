import React from 'react';
import defaultPage from './defaultPage';

const securePageHoc = Page => class SecurePage extends React.Component {
  static async getInitialProps (ctx) {
    return Page.getInitialProps && Page.getInitialProps(ctx);
  }

  render () {
    return <Page { ...this.props } />
  }
}

export default Page => defaultPage(securePageHoc(Page));
