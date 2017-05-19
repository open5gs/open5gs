import React from 'react'
import Session from './session'

export default class extends React.Component {

  // Expose session to all pages
  static async getInitialProps({req}) {
    const session = new Session({req})
    return {session: await session.getSession()}
  }

}
