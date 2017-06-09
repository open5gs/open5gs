import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import Router from 'next/router';
import NProgress from 'nprogress';

import Session from 'lib/session';

import * as authActions from 'actions/auth';
import * as uiActions from 'actions/ui';

import { Login } from 'components';

class Auth extends Component {
  state = {
    error: null,
    form: {
      username: '',
      password: ''
    },
    session: {}
  };

  static async getInitialProps({req}) {
    const session = new Session({req})
    return {session: await session.getSession(true)}
  }

  async componentDidMount() {
    const session = new Session()
    this.setState({ 
      session: await session.getSession(true)
    });

    this.input.focus();
  }

  setInnerRef = (comp) => {
    this.input = comp;
  }

  handleChange = (e) => {
    this.setState({ 
      form: {
        ...this.state.form,
        [e.target.name]: e.target.value 
      }
    });
  }

  handleKeyPress = (e) => {
    if (e.charCode === 13) {
        this.handleSubmit();
    }
  }

  handleSubmit = (e) => {
    const { 
      username,
      password
    } = this.state.form;

    NProgress.configure({ showSpinner: false });
    NProgress.start();

    const session = new Session()
    session.signin(username, password)
      .then(() => {
        NProgress.done();

        Router.push('/');
      })
      .catch(err => {
        NProgress.done();

        this.setState({
          error: { 
            message: err.message
          }
        });

        this.input.focus();
      })
  }

  handleErrorReset = (e) => {
    this.setState({
      error: null
    });
  }

  render() {
    const { 
      form,
      error
    } = this.state;

    const { 
      setInnerRef,
      handleChange,
      handleSubmit,
      handleKeyPress,
      handleErrorReset
    } = this;

    return (
      <Login
        form={form}
        error={error}
        innerRef={setInnerRef}
        onChange={handleChange}
        onSubmit={handleSubmit}
        onKeyPress={handleKeyPress}
        onErrorReset={handleErrorReset}
      />
    );
  }
}

Auth = connect(
  null,
  (dispatch) => ({
    UIActions: bindActionCreators(uiActions, dispatch)
  })
)(Auth);

export default Auth;