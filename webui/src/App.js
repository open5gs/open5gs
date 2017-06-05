import Package from '../package';

import { Component } from 'react';
import Head from 'next/head';
import PropTypes from 'prop-types';

import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';
import compose from 'recompose/compose';

import Session from './lib/session';
import withWidth, { SMALL } from './lib/with-width';
import * as sidebarActions from './modules/sidebar';

import styled from 'styled-components';
import oc from 'open-color';
import { media, transitions } from './lib/style-utils';

import Header from './components/Header';
import Sidebar from './components/Sidebar';
import Container from './components/Container';
import PdnContainer from './containers/PdnContainer';
import UserContainer from './containers/UserContainer';

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
  static propTypes = {
    session: PropTypes.string.isRequired,
    width: PropTypes.number.isRequired
  }

  componentWillMount() {
    const { 
      width,
      SidebarActions
    } = this.props;

    if (width !== SMALL) {
      SidebarActions.setVisibility(true);
    }
  }

  handleToggle = () => {
    const { SidebarActions } = this.props;
    SidebarActions.toggle();
  }

  handleSelect = (view) => {
    const { 
      width,
      SidebarActions
    } = this.props;

    SidebarActions.setView(view);
    if (width === SMALL) {
      SidebarActions.toggle();
    }
  }

  render() {
    const title = 'Next.EPC ' + Package.version;
    const session = this.props.session;

    const {
      visible,
      view
    } = this.props;

    const {
      handleToggle,
      handleSelect
    } = this;

    return (
      <div>
        <Head>
          <title>{title}</title>
        </Head>
        <Header onMenuAction={handleToggle}/>
        <Body>
          <Sidebar 
            visible={visible}
            selected={view}
            onSelect={handleSelect}/>
          <Container visible={view === 'pdn'}>
            <PdnContainer/>
          </Container>
          <Container visible={view === 'user'}>
            <UserContainer/>
          </Container>
          <Container visible={view === 'test1'}>
            <Content>{view}</Content>
          </Container>
          <Container visible={view === 'test3'}>
            <Content>{view}</Content>
          </Container>
        </Body>
      </div>
    )
  }
}

const enhance = compose(
  withWidth(),
  connect(
    (state) => ({
      visible: state.sidebar.get('visible'),
      view: state.sidebar.get('view')
    }),
    (dispatch) => ({
      SidebarActions: bindActionCreators(sidebarActions, dispatch)
    })
  )
);

export default enhance(App);
