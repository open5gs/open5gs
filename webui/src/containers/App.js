import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { bindActionCreators, compose } from 'redux';

import * as uiActions from 'modules/ui';
import withWidth, { SMALL } from 'helpers/with-width';

import { Layout } from 'components';
import SubscriberContainer from 'containers/SubscriberContainer';
import PdnContainer from 'containers/PdnContainer';
import UserContainer from 'containers/UserContainer';

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

  render() {
    const {
      view
    } = this.props;

    if (view === "subscriber") {
      document.body.style.backgroundColor = "#e9ecef";
    } else {
      document.body.style.backgroundColor = "white";
    }

    return (
      <Layout>
        <Layout.Container visible={view === "subscriber"}>
          <SubscriberContainer/>
        </Layout.Container>
        <Layout.Container visible={view === "pdn"}>
          <Layout.Content><PdnContainer/></Layout.Content>
        </Layout.Container>
        <Layout.Container visible={view === "user"}>
          <Layout.Content><UserContainer/></Layout.Content>
        </Layout.Container>
        <Layout.Container visible={view === "test1"}>
          <Layout.Content>{view}</Layout.Content>
        </Layout.Container>
        <Layout.Container visible={view === "test3"}>
          <Layout.Content>{view}</Layout.Content>
        </Layout.Container>
      </Layout>
    )
  }
}

const enhance = compose(
  withWidth(),
  connect(
    (state) => ({
      view: state.ui.sidebar.view
    }),
    (dispatch) => ({
      UIActions: bindActionCreators(uiActions, dispatch)
    })
  )
);

export default enhance(App);
