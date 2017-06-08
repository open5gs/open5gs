import { Component } from 'react';
import PropTypes from 'prop-types';

import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';

import * as uiActions from '../actions/ui';

import Session from '../lib/session';

import Header from '../components/Header';
import Logout from '../components//Logout';
import Dimmed from '../components//Dimmed';

class HeaderContainer extends Component {
  state = {
    modal: {
      visible: false,
      dimmed: false
    }
  }

  modalHandler = {
    show: () => {
      this.setState({
        modal: {
          visible: true,
          dimmed: true
        }
      })
    },
    hide: () => {
      this.setState({
        modal: {
          visible: false,
          dimmed: false
        }
      })
    },
    logout: async () => {
      this.setState({
        modal: {
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
    const { UIActions } = this.props;
    UIActions.toggleSidebar();
  }

  render() {
    const {
      handleSidebarToggle,
      modalHandler
    } = this;

    const {
      modal
    } = this.state;

    return (
      <div>
        <Header 
          onSidebarToggle={handleSidebarToggle}
          onLogoutRequest={modalHandler.show}
        />
        <Logout 
          visible={modal.visible}
          onHide={modalHandler.hide}
          onLogout={modalHandler.logout} />
        <Dimmed visible={modal.dimmed} />
      </div>
    )
  }
}

export default connect(
  null,
  (dispatch) => ({
    UIActions: bindActionCreators(uiActions, dispatch)
  })
)(HeaderContainer);