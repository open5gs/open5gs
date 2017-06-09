import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { bindActionCreators, compose } from 'redux';

import withWidth, { SMALL } from 'lib/with-width';
import * as uiActions from 'actions/ui';

import { Sidebar } from 'components';

class SidebarContainer extends Component {
  static propTypes = {
    width: PropTypes.number.isRequired,
    isOpen: PropTypes.bool.isRequired,
    view: PropTypes.string.isRequired
  }

  handleSelectView = (view) => {
    const { 
      width,
      UIActions
    } = this.props;

    UIActions.selectView(view);
    if (width === SMALL) {
      UIActions.toggleSidebar();
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
      isOpen: state.ui.sidebar.isOpen,
      view: state.ui.sidebar.view
    }),
    (dispatch) => ({
      UIActions: bindActionCreators(uiActions, dispatch)
    })
  )
);

export default enhance(SidebarContainer);