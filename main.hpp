/**
 * \mainpage
 * \author Mathias Laurin <mathias_laurin@users.sourceforge.net>
 * \date 2008-11-13
 *
 *
 * \section Abstract
 *
 * dmcut2 is a scientific program designed to simulate chemical reactions
 * at surfaces using Monte-Carlo (MC) methods 
 * [see, e.g., <a href="http://babbage.sissa.it/abs/cond-mat/0303028v1">Jansen's Introduction to Monte Carlo Simulations of Surface Reactions </a>]. 
 * The program also features a <a href="http://dx.doi.org/10.1063/1.1699114">Metropolis walk algorithm</a>.
 * Short ranged interactions between adsorbates may be taken into account.
 *
 * During development, emphasis was put on coding style and code readability. 
 * Efficiency (speed) was considered during the implementation of the algorithms.
 *
 * \section Motivations
 * MC simulations have a broad range of applications in many aspects of
 *  physical chemistry at surfaces, mostly chemical reactions and magnetism. 
 *  MC simulations are a way to efficiently integrate the master equation. 
 *  MC simulations are also the only way to simulate transient states of complex reactions.
 *
 * This program differs from the existing programs by the attention that 
 * has been ported to coding style and adherence to coding standards. 
 * Indeed, surface MC programs are typically developed by students with little 
 * or no interest in programming. 
 * Such code is also generally kept in house and cannot be peer reviewed. 
 * Adding to that, such code is unfortunately not general enough to be used 
 * for slightly different scenario.
 *
 * dmcut2 aims at providing code that is both designed with surface MC 
 * in mind as well as adherence to coding standards. 
 * This codebase is meant to be used by students without an extensive 
 * knowledge of programming techniques so that they have a solid base 
 * to adapt to the model they are working on. 
 * Open sourcing the code is also a way to allow peer reviewing so that 
 * results from this programs can be trusted by the scientific community. 
 *
 * \section Acknowledgement
 * ML gratefully acknowledges the Japanese Society for the 
 * Promotion of Science (JSPS) for a twenty four months 
 * postdoctoral fellowship at the University of Tokyo and at the Keio University under the supervision of prof. Kondoh.
 *
 * \section Appendix Dependences
 *
 * <a href="http://charles.karney.info/random/">RandomLib</a>,
 * a C++ interface to the Mersenne Twister random number generator 
 * <a href="http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html">MT19937</a>
 *  and to the SIMD-oriented Fast Mersenne Twister random number generator,
 * <a href="http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/index.html">
 * SFMT19937</a> is used as the random number generator for this program.
 *
 * Libraries from the 
 * <a href="http://en.wikipedia.org/wiki/Technical_Report_1">C++ Technical Report 1</a> 
 * <a href="http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2005/n1745.pdf">TR1</a>
 * are also used when appropriate. Implementations are available in GCC 4
 * and above, from 
 * <a href="http://www.boost.org/">boost</a> 
 * or 
 * <a href="https://sourceforge.net/projects/stlport/">STLPort</a>.
 */
