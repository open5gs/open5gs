import { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import { fetchSubscribers } from 'modules/crud/subscriber';
import { select } from 'modules/crud/selectors';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import { Spinner, Subscriber } from 'components';

const Wrapper = styled.div`
  width: 100%;
  height: 100%;
  padding: 2rem 0rem;

  background: #e9ecef;

  ${media.mobile`
     padding: 0rem;
  `}
`

class SubscriberContainer extends Component {
  state = {
    search: ''
  };

  componentWillMount() {
    const { subscribers, dispatch } = this.props
    if (subscribers.needsFetch) {
      dispatch(subscribers.fetch)
    }
  }

  componentWillReceiveProps(nextProps) {
    const { subscribers } = nextProps
    const { dispatch } = this.props
    if (subscribers.needsFetch) {
      dispatch(subscribers.fetch)
    }
  }

  handleSearchChange = (e) => {
    this.setState({
      search: e.target.value
    });
  }

  handleSearchClear = (e) => {
    this.setState({
      search: ''
    });
  }

  render() {
    const {
      handleSearchChange,
      handleSearchClear
    } = this;

    const { 
      search
    } = this.state;

    const { 
      subscribers 
    } = this.props

    return (
      <Wrapper>
        <Subscriber.Search 
          onChange={handleSearchChange}
          value={search}
          onClear={handleSearchClear} />
        <Subscriber.List
          subscribers={subscribers.data}
          search={search}
        />
        {subscribers.isLoading && <Spinner md color={oc.indigo[9]} />}
      </Wrapper>
    )
  }
}

function mapStateToProps(state, ownProps) {
  return { subscribers: select(fetchSubscribers(), state.crud) }
}

export default connect(mapStateToProps)(SubscriberContainer)