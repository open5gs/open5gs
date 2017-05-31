import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';

const Wrapper = styled.div`
  width: ${props => props.width};
  z-index: 1;

  background-color: ${oc.indigo[3]};
  border-right: 1px solid ${oc.indigo[4]};
  box-shadow: 3px 3px 6px rgba(0,0,0,0.10), 3px 3px 6px rgba(0,0,0,0.20);
`;

const Sidebar = ({ visible, width }) => visible ? (
  (<Wrapper width={width}> </Wrapper>)
) : null;

Sidebar.propTypes = {
  visible: PropTypes.bool,
  width: PropTypes.string
};

Sidebar.defaultProps = {
    visible: true,
    width: '16rem'
};

export default Sidebar;
