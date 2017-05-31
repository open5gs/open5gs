import PropTypes from 'prop-types';

import styled from 'styled-components';
import { media, transitions} from '../lib/style-utils';
import oc from 'open-color';

const Wrapper = styled.div`
  z-index: 1;
  width: ${p => p.toggled ? '0' : p.width };
  transition: width .2s ease-in-out;

  ${media.mobile`
    width: 100%;
    height: ${p => p.toggled ? '100%' : '0'};
    transition: height .2s ease-in-out;
  `}

  background-color: ${oc.indigo[3]};
  border-right: 1px solid ${oc.indigo[4]};
  box-shadow: 3px 3px 6px rgba(0,0,0,0.10), 3px 3px 6px rgba(0,0,0,0.20);
`;

const Sidebar = ({ toggled, width }) => (
  <Wrapper toggled={toggled} width={width}>
  </Wrapper>
)

Sidebar.propTypes = {
  toggled: PropTypes.bool,
  width: PropTypes.string
};

Sidebar.defaultProps = {
  toggled: false,
  width: '16rem'
};

export default Sidebar;
