import { Component } from 'react';
import PropTypes from 'prop-types';

import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';

import * as sidebarActions from '../modules/sidebar';

import Session from '../lib/session';

import Header from '../components/Header';
import Logout from '../components//Logout';
import Dimmed from '../components//Dimmed';

class HeaderContainer extends Component {
  state = {
    logout: {
      visible: false,
      dimmed: false
    }
  }

  logoutHandler = {
    show: () => {
      this.setState({
        logout: {
          ...this.state.logout,
          visible: true,
          dimmed: true
        }
      })
    },
    hide: () => {
      this.setState({
        logout: {
          ...this.state.logout,
          visible: false,
          dimmed: false
        }
      })
    },
    action: async () => {
      this.setState({
        logout: {
          ...this.state.logout,
          visible: false,
        }
      })

      const session = new Session()
      await session.signout()

      // @FIXME next/router not working reliably  so using window.location
      window.location = '/'
    }
  }

  handleSidebarToggle = () => {
    const { SidebarActions } = this.props;
    SidebarActions.toggle();
  }

  render() {
    const {
      handleSidebarToggle,
      logoutHandler
    } = this;

    const {
      logout
    } = this.state;

    return (
      <div>
        <Header 
          onSidebarToggle={handleSidebarToggle}
          onLogoutRequest={logoutHandler.show}
        />
        <Logout 
          {...logout} 
          onHide={logoutHandler.hide}
          onAction={logoutHandler.action} />
        <Dimmed visible={logout.dimmed} />
      </div>
    )
  }
}

export default connect(
  null,
  (dispatch) => ({
    SidebarActions: bindActionCreators(sidebarActions, dispatch)
  })
)(HeaderContainer);