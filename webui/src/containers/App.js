import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { bindActionCreators, compose } from 'redux';

import * as uiActions from 'modules/ui';
import withWidth, { SMALL } from 'helpers/with-width';

import { Layout } from 'components';
import * as Subscriber from 'containers/Subscriber';

import Notifications from './notifications';

class App extends Component {
  static propTypes = {
    session: PropTypes.object.isRequired,
    view: PropTypes.string.isRequired,
    width: PropTypes.number.isRequired
  }
  
  componentWillMount() {
    const { 
      width,
      UIActions
    } = this.props;

    if (width !== SMALL) {
      UIActions.setSidebarVisibility(true);
    }
  }

  componentWillReceiveProps(nextProps) {
    console.log(nextProps);
  }

  render() {
    const {
      view,
      notification
    } = this.props;

    if (view === "subscriber") {
      document.body.style.backgroundColor = "#e9ecef";
    } else {
      document.body.style.backgroundColor = "white";
    }

    return (
      <Layout>
        <Layout.Container visible={view === "subscriber"}>
          <Subscriber.Collection/>
        </Layout.Container>
        <Layout.Container visible={view === "pdn"}>
          <Layout.Content>{view}</Layout.Content>
        </Layout.Container>
        <Layout.Container visible={view === "user"}>
          <Layout.Content>{view}</Layout.Content>
        </Layout.Container>
        <Layout.Container visible={view === "test1"}>
          <Layout.Content>{view}</Layout.Content>
        </Layout.Container>
        <Layout.Container visible={view === "test3"}>
          <Layout.Content>{view}</Layout.Content>
        </Layout.Container>
        <Notifications notifications={notification} />
      </Layout>
    )
  }
}

const enhance = compose(
  withWidth(),
  connect(
    (state) => ({
      view: state.ui.sidebar.view,
      notification: state.notification
    }),
    (dispatch) => ({
      UIActions: bindActionCreators(uiActions, dispatch)
    })
  )
);

export default enhance(App);
