import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import { MODEL, fetchProfiles, deleteProfile } from 'modules/crud/profile';
import { clearActionStatus } from 'modules/crud/actions';
import { select, selectActionStatus } from 'modules/crud/selectors';
import * as Notification from 'modules/notification/actions';

import { 
  Layout, 
  Profile, 
  Spinner, 
  FloatingButton, 
  Blank,
  Dimmed,
  Confirm
} from 'components';

import Document from './Document';

class Collection extends Component {
  state = {
    document: {
      action: '',
      visible: false,
      dimmed: false
    },
    confirm: {
      visible: false,
      _id: ''
    },
    view: {
      visible: false,
      disableOnClickOutside: false,
      _id: ''
    }
  };

  componentWillMount() {
    const { profiles, dispatch } = this.props

    if (profiles.needsFetch) {
      dispatch(profiles.fetch)
    }
  }

  componentWillReceiveProps(nextProps) {
    const { profiles, status } = nextProps
    const { dispatch } = this.props

    if (profiles.needsFetch) {
      dispatch(profiles.fetch)
    }

    if (status.response) {
      dispatch(Notification.success({
        title: 'Profile',
//        message: `${status.id} has been deleted`
        message: `This profile has been deleted`
      }));
      dispatch(clearActionStatus(MODEL, 'delete'));
    } 

    if (status.error) {
      let title = 'Unknown Code';
      let message = 'Unknown Error';
      if (response.data && response.data.name && response.data.message) {
        title = response.data.name;
        message = response.data.message;
      } else {
        title = response.status;
        message = response.statusText;
      }

      dispatch(Notification.error({
        title,
        message,
        autoDismiss: 0,
        action: {
          label: 'Dismiss'
        }
      }));
      dispatch(clearActionStatus(MODEL, 'delete'));
    }
  }

  documentHandler = {
    show: (action, payload) => {
      this.setState({
        document: {
          action,
          visible: true,
          dimmed: true,
          ...payload
        },
        view: {
          ...this.state.view,
          disableOnClickOutside: true
        }
      })
    },
    hide: () => {
      this.setState({
        document: {
          action: '',
          visible: false,
          dimmed: false
        },
        view: {
          ...this.state.view,
          disableOnClickOutside: false
        }
      })
    },
    actions: {
      create: () => {
        this.documentHandler.show('create');
      },
      update: (_id) => {
        this.documentHandler.show('update', { _id });
      }
    }
  }

  confirmHandler = {
    show: (_id) => {
      this.setState({
        confirm: {
          visible: true,
          _id,
        },
        view: {
          ...this.state.view,
          disableOnClickOutside: true
        }
      })
    },
    hide: () => {
      this.setState({
        confirm: {
          ...this.state.confirm,
          visible: false
        },
        view: {
          ...this.state.view,
          disableOnClickOutside: false
        }
      })
    },
    actions : {
      delete: () => {
        const { dispatch } = this.props

        if (this.state.confirm.visible === true) {
          this.confirmHandler.hide();
          this.documentHandler.hide();
          this.viewHandler.hide();

          dispatch(deleteProfile(this.state.confirm._id));
        }
      }
    }
  }

  viewHandler = {
    show: (_id) => {
      this.setState({
        view: {
          _id,
          visible: true,
          disableOnClickOutside: false
        }
      });
    },
    hide: () => {
      this.setState({
        view: {
          ...this.state.view,
          visible: false
        }
      })
    }
  }


  render() {
    const {
      documentHandler,
      viewHandler,
      confirmHandler
    } = this;

    const { 
      document
    } = this.state;

    const { 
      profiles,
      status
    } = this.props

    const {
      isLoading,
      data
    } = profiles;

    return (
      <Layout.Content>
        <Profile.List
          profiles={data}
          deletedId={status.id}
          onView={viewHandler.show}
          onEdit={documentHandler.actions.update}
          onDelete={confirmHandler.show}
        />
        {isLoading && <Spinner md />}
        <Blank
          visible={!isLoading && !(Object.keys(data).length > 0)}
          title="ADD A PROFILE"
          body="You have no profiles... yet!"
          onTitle={documentHandler.actions.create}
          />
        <FloatingButton onClick={documentHandler.actions.create}/>
        <Profile.View
          visible={this.state.view.visible}
          profile={data.filter(profile => 
            profile._id === this.state.view._id)[0]}
          disableOnClickOutside={this.state.view.disableOnClickOutside}
          onEdit={documentHandler.actions.update}
          onDelete={confirmHandler.show}
          onHide={viewHandler.hide}/>
        <Document 
          { ...document }
          onEdit={documentHandler.actions.update}
          onDelete={confirmHandler.show}
          onHide={documentHandler.hide} />
        <Dimmed visible={document.dimmed} />
        <Confirm
          visible={this.state.confirm.visible}
          message="Delete this profile?"
          onOutside={confirmHandler.hide}
          buttons={[
            { text: "CANCEL", action: confirmHandler.hide, info:true },
            { text: "DELETE", action: confirmHandler.actions.delete, danger:true }
          ]}/>
      </Layout.Content>
    )
  }
}

Collection = connect(
  (state) => ({ 
    profiles: select(fetchProfiles(), state.crud),
    status: selectActionStatus(MODEL, state.crud, 'delete')
  })
)(Collection);

export default Collection;
