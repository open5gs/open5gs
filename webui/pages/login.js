import React from 'react'
import Router from 'next/router'
import Session from '../lib/session'

export default class extends React.Component {

  static async getInitialProps({req}) {
    // On the sign in page we always force get the latest session data from the
    // server by passing 'true' to getSession. This page is the destination
    // page after logging or linking/unlinking accounts so avoids any weird
    // edge cases.
    const session = new Session({req})
    return {session: await session.getSession(true)}
  }

  async componentDidMount() {
    // Get latest session data after rendering on client
    // Any page that is specified as the oauth callback should do this
    const session = new Session()
    this.state = {
      username: this.state.username,
      password: this.state.password,
      session: await session.getSession(true)
    }
  }

  constructor(props) {
    super(props)
    this.state = {
      username: '',
      password: '',
      session: this.props.session
    }
    this.handleSubmit = this.handleSubmit.bind(this)
    this.handleUsernameChange = this.handleUsernameChange.bind(this)
    this.handlePasswordChange = this.handlePasswordChange.bind(this)
  }

  handleUsernameChange(event) {
    this.setState({
      username: event.target.value.trim(),
      password: this.state.password,
      session: this.state.session
    })
  }

  handlePasswordChange(event) {
    this.setState({
      username: this.state.username,
      password: event.target.value.trim(),
      session: this.state.session
    })
  }

  async handleSubmit(event) {
    event.preventDefault()

    const session = new Session()
    session.signin(this.state.username, this.state.password)
    .then(() => {
      this.props.url.push('/');
    })
    .catch(err => {
      // @FIXME Handle error
      console.log(err)
    })
  }

  render() {
    let signinForm = <div/>
    if (this.state.session.user) {
      signinForm = (
        <div>
          <h3>You are signed in</h3>
          <p>Name: <strong>{this.state.session.user.name}</strong></p>
        </div>
      )
    } else {
      signinForm = (
        <div>
          <form id="signin" method="post" action="/api/auth/login" onSubmit={this.handleSubmit}>
            <input name="_csrf" type="hidden" value={this.state.session.csrfToken}/>
            <h3>Sign in with email</h3>
            <p>
              <label htmlFor="username">Username</label><br/>
              <input name="username" type="text" placeholder="j.smith@example.com" id="username" value={this.state.username} onChange={this.handleUsernameChange}/>
            </p>
            <p>
              <label htmlFor="password">Password</label><br/>
              <input name="password" type="text" id="password" value={this.state.password} onChange={this.handlePasswordChange}/>
            </p>
            <p>
              <button id="submitButton" type="submit">Sign in</button>
            </p>
          </form>
        </div>
      )
    }

    return (
      <div>
        <h2>Authentication</h2>
        {signinForm}
      </div>
    )
  }

}