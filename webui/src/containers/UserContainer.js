import { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

import ViewSelector from '../components/User/ViewSelector';

const Wrapper = styled.div`
`;

class UserContainer extends Component {
  state = {
    view: 'user'
  }
  handleSelectView = (view) => this.setState({view});
  render() {
    const {
      handleSelectView
    } = this;

    const {
      view
    } = this.state;

    return (
      <Wrapper>
        <ViewSelector onSelect={handleSelectView} selected={view}/>
      </Wrapper>
    )
  }
}

export default UserContainer;
