import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import Layout from '../components/Layout';
import Container from '../components/Container';
import PdnContainer from '../containers/PdnContainer';
import UserContainer from '../containers/UserContainer';

import Package from '../../package';

const App = ({ session, view }) => (
  <Layout title={`Next.EPC ${Package.version}`}>
    <Container visible={view === 'pdn'}>
      <PdnContainer/>
    </Container>
    <Container visible={view === 'user'}>
      <UserContainer/>
    </Container>
    <Container visible={view === 'test1'}>
      <Layout.Content>{view}</Layout.Content>
    </Container>
    <Container visible={view === 'test3'}>
      <Layout.Content>{view}</Layout.Content>
    </Container>
  </Layout>
)

App.propTypes = {
  session: PropTypes.object.isRequired,
}

export default connect(
  (state) => ({
    view: state.sidebar.get('view')
  })
)(App);