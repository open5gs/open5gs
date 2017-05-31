import PropTypes from 'prop-types';

import styled from 'styled-components';
import { media, transitions} from '../lib/style-utils';
import oc from 'open-color';

const Wrapper = styled.div`
  z-index: 1;
  width: ${p => p.visible ? p.width : '0'};
  transition: width .2s ease-in-out;

  background-color: ${oc.indigo[3]};
  border-right: 1px solid ${oc.indigo[4]};
  box-shadow: 3px 3px 6px rgba(0,0,0,0.10), 3px 3px 6px rgba(0,0,0,0.20);
`;

const Sidebar = ({ visible, width }) => 
  (<Wrapper visible={visible} width={width}> </Wrapper>)

Sidebar.propTypes = {
  visible: PropTypes.bool,
  width: PropTypes.string
};

Sidebar.defaultProps = {
    visible: true,
    width: '16rem'
};

export default Sidebar;
