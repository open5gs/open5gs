import { Component } from 'react';
import PropTypes from 'prop-types';

import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import compose from 'recompose/compose';

import withWidth, { SMALL } from '../lib/with-width';
import * as sidebarActions from '../modules/sidebar';

import Sidebar from '../components/Sidebar';

class SidebarContainer extends Component {
  static propTypes = {
    width: PropTypes.number.isRequired,
    isOpen: PropTypes.bool.isRequired,
    view: PropTypes.string.isRequired
  }

  handleSelectView = (view) => {
    const { 
      width,
      SidebarActions
    } = this.props;

    SidebarActions.selectView(view);
    if (width === SMALL) {
      SidebarActions.toggle();
    }
  }

  render() {
    const {
      isOpen,
      view
    } = this.props;

    const {
      handleSelectView
    } = this;

    return (
      <Sidebar 
        isOpen={isOpen}
        selectedView={view}
        onSelectView={handleSelectView}/>
    )
  }
}

const enhance = compose(
  withWidth(),
  connect(
    (state) => ({
      isOpen: state.sidebar.get('isOpen'),
      view: state.sidebar.get('view')
    }),
    (dispatch) => ({
      SidebarActions: bindActionCreators(sidebarActions, dispatch)
    })
  )
);

export default enhance(SidebarContainer);