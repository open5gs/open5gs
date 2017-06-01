import Package from '../package';
import { Component } from 'react';
import Head from 'next/head';

import styled from 'styled-components';
import oc from 'open-color';
import { media, transitions } from '../lib/style-utils';

import Header from './Header';
import Sidebar from './Sidebar';

import Session from '../lib/session';

const BodyContainer = styled.div`
  display: flex;
  ${media.mobile`
    display: block;
  `}
  height: calc(100vh - 4rem);
`
const ContentContainer = styled.div`
  flex: 1;
  ${media.mobile`
    position: absolute;
    top: 4rem;
    left: 0;
    width: 100%;
    z-index: -1;
  `}
  padding: 1rem;
`;

const HelloWorld = styled.div`
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
      toggled: false
    },
    modal: {
      visible: false
    },
    error: {
      status: false,
      message: ''
    },
  };

  onToogleSidebar = (e) => {
    this.setState({
      sidebar: {
        ...this.state.sidebar,
        toggled: !this.state.sidebar.toggled
      }
    })
  }
  
  render() {
    const title = 'Next, EPC ' + Package.version;
    const session = this.props.session;

    return (
      <div>
        <Head>
          <title>{title}</title>
        </Head>

        <Header onMenuAction={this.onToogleSidebar}/>
        <BodyContainer>
          <Sidebar toggled={this.state.sidebar.toggled}/>
          <ContentContainer>
            <HelloWorld>
              Hello, World
            </HelloWorld>
          </ContentContainer>
        </BodyContainer>
      </div>
    )
  }
}

export default App;
