import React from 'react';
import withSession from '../components/with-session';
import Session from '../components/session';

class Login extends React.Component {
  constructor (props) {
    super(props);
    this.state = {
      username: '',
      password: ''
    };
    this.handleSubmit = this.handleSubmit.bind(this);
    this.handleUsernameChange = this.handleUsernameChange.bind(this);
    this.handlePasswordChange = this.handlePasswordChange.bind(this);
  }

  handleUsernameChange(event) {
    this.setState({ 
      username: event.target.value.trim(),
      password: this.state.password
     });
  }
  handlePasswordChange(event) {
    this.setState({ 
      username: this.state.username,
      password: event.target.value.trim() 
    });
  }

  async handleSubmit(evnet) {
    event.preventDefault();

    const session = new Session();
    session
      .login(this.username, this.password)
      .then(() => {
        this.props.url.push('/');
      })
      .catch(err => {
        console.log(err);
      });
  }

  render() {
    let loginForm = <div />;
    if (!this.props.session.user) {
      loginForm = (
        <div>
          <form id='login' method='post' action='/login' onSubmit={this.handleSubmit}>
            <input name='_csrf' type='hidden' value={this.props.session.csrfToken} />
            <h3>Login</h3>
            <p>
              <label htmlFor='username'>Username</label><br />
              <input name='username' type='text' id='username' value={this.state.username} onChange={this.handleUsernameChange} />
            </p>
            <p>
              <label htmlFor='password'>Password</label><br />
              <input name='password' type='text' id='password' value={this.state.password} onChange={this.handlePasswordChange} />
            </p>
            <p>
              <button id='submitButton' type='submit'>Login</button>
            </p>
          </form>
        </div>
      )
    }

    return (
      <div>
        <h2>Authentication</h2>
        {loginForm}
      </div>
    )
  }
}

export default withSession(Login);