import React from 'react';

class Login extends React.Component {
  render() {
      return (
        <div>
          <form id='login' method='post' action='/login'>
            <div>
              <label>Username:</label>
              <input name='username' type='text' id='username'/>
            </div>
            <div>
              <label>Password:</label>
              <input name='password' type='text' id='password'/>
            </div>
            <div>
              <input type='submit' value='Log In'/>
            </div>
          </form>
        </div>
      )
  }
}

export default (Login);