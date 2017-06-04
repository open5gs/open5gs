import Package from '../package';
import withWidth, { SMALL } from '../lib/with-width';

import { Component } from 'react';
import Head from 'next/head';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media, transitions } from '../lib/style-utils';

import Header from './Header';
import Sidebar from './Sidebar';
import Container from './Container';
import PdnContainer from './PdnContainer';
import UserContainer from './UserContainer';

import Session from '../lib/session';

const Body = styled.div`
  display: flex;
  height: calc(100vh - 4rem);
`
const Content = styled.div`
  display: flex;
  align-items: center;
  justify-content: center;

  font-size: 4rem;
  line-height: 8rem;
  color: ${oc.gray[5]};

  border-top: 1px solid ${oc.gray[4]};
  box-shadow: 3px 3px 6px rgba(0,0,0,0.10), 3px 3px 6px rgba(0,0,0,0.20);

  background-color: white;
`;

class App extends Component {
  state = {
    sidebar: {
      visible: this.props.width === SMALL ? false : true,
      view: "user"
    },
    error: {
      status: false,
      message: ''
    },
  };

  static propTypes = {
    session: PropTypes.string.isRequired,
    width: PropTypes.number.isRequired
  }

  sidebarHandler = {
    toggle: () => {
      this.setState({
        sidebar: {
          ...this.state.sidebar,
          visible: !this.state.sidebar.visible
        }
      })
    },
    selectView: (view) => {
      this.setState({
        sidebar: {
          ...this.state.sidebar,
          visible: this.props.width === SMALL ? 
                    !this.state.sidebar.visible : 
                    this.state.sidebar.visible,
          view: view
        }
      })
    }
  }

  render() {
    const title = 'Next, EPC ' + Package.version;
    const session = this.props.session;

    const {
      sidebarHandler
    } = this;

    const {
      sidebar 
    } = this.state;

    return (
      <div>
        <Head>
          <title>{title}</title>
        </Head>
        <Header onMenuAction={sidebarHandler.toggle}/>
        <Body>
          <Sidebar 
            visible={sidebar.visible}
            selected={sidebar.view}
            onSelect={sidebarHandler.selectView}/>
          <Container visible={sidebar.view === 'pdn'}>
            <PdnContainer/>
          </Container>
          <Container visible={sidebar.view === 'user'}>
            <UserContainer/>
          </Container>
          <Container visible={sidebar.view === 'test1'}>
            <Content>{sidebar.view}</Content>
          </Container>
          <Container visible={sidebar.view === 'test3'}>
            <Content>{sidebar.view}</Content>
          </Container>
        </Body>
      </div>
    )
  }
}

export default withWidth()(App);
