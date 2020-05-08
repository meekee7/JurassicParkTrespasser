/***********************************************************************************************
 *
 * Copyright � DreamWorks Interactive, 1997.
 *
 * Contents:
 *		Specialized (non-template) versions of the DrawSubtriangle function for terrain.
 *		Optimized for the Pentium Processor
 *
 ***********************************************************************************************
 *
 * $Log:: /JP2_PC/Source/Lib/Renderer/Primitives/P5/DrawSubTriangleWater.cpp                   $
 * 
 * 4     98.08.13 4:26p Mmouni
 * Changes for VC++ 5.0sp3 compatibility.
 * 
 * 3     98.04.30 5:22p Mmouni
 * Added stippled texture primitives.
 * 
 * 2     97.11.20 4:00p Mmouni
 * Optimized for Pentium.
 * 
 * 1     97/11/09 7:07p Pkeet
 * Initial implementation.
 * 
 **********************************************************************************************/

#include "Common.hpp"
#include "Lib/View/Raster.hpp"
#include "Lib/Renderer/Primitives/DrawTriangle.hpp"

// Turn off the global optimizations to prevent re-arranging of the assembly code.
#pragma optimize("g", off)


//
// Globals.
//

// Values supplied by the light blend module.
extern uint16  u2WaterMask;
extern uint16* pu2WaterAlpha;

// Zero extended version of u2WaterMask.
static uint32 u4WaterMask;

// u2WaterMask duplicated across both words of a dword.
static uint32 u4DupWaterMask;


//
// Function implementations.
//

//*****************************************************************************************
//
// Perspective, alpha textured, 16 bit mode.
//
void DrawSubtriangle(TWaterPersp* pscan, CDrawPolygon<TWaterPersp>* pdtri)
{
	typedef TWaterPersp::TIndex prevent_internal_compiler_errors;
	static void* pvLastClut = 0;
	static void* pvLastTexture = 0;

	TWaterPersp* plinc = &pdtri->pedgeBase->lineIncrement;

	Assert(pu2WaterAlpha);

	__asm
	{
		//
		// Do self modification.
		//
		mov		ecx,[pvTextureBitmap]				// Texture pointer.
		mov		edx,[pu2WaterAlpha]					// Clut conversion pointer.
		
		mov		eax,[pvLastTexture]
		mov		ebx,[pvLastClut]

		cmp		eax,ecx
		jne		short DO_MODIFY

		cmp		ebx,edx
		je		short DONE_WITH_MODIFY

DO_MODIFY:
		mov		[pvLastTexture],ecx					// Update last texture pointer.
		mov		[pvLastClut],edx					// Update last clut pointer.

		mov	eax,ecx									// Texture pointer plus one.
		inc	ecx
		mov	ebx,edx									// Clut pointer plus one.
		inc	edx

		lea		esi,MODIFY_FOR_TEXTUREPOINTER_E
		mov		[esi-4],eax
		lea		esi,MODIFY_FOR_TEXTUREPOINTER_P1_E
		mov		[esi-4],ecx
		lea		esi,MODIFY_FOR_CLUT_E
		mov		[esi-4],ebx
		lea		esi,MODIFY_FOR_CLUT_P1_E
		mov		[esi-4],edx

		lea		esi,MODIFY_FOR_TEXTUREPOINTER_F
		mov		[esi-4],eax
		lea		esi,MODIFY_FOR_TEXTUREPOINTER_P1_F
		mov		[esi-4],ecx
		lea		esi,MODIFY_FOR_CLUT_F
		mov		[esi-4],ebx
		lea		esi,MODIFY_FOR_CLUT_P1_F
		mov		[esi-4],edx

		lea		esi,MODIFY_FOR_TEXTUREPOINTER_G
		mov		[esi-4],eax
		lea		esi,MODIFY_FOR_TEXTUREPOINTER_P1_G
		mov		[esi-4],ecx
		lea		esi,MODIFY_FOR_CLUT_G
		mov		[esi-4],ebx
		lea		esi,MODIFY_FOR_CLUT_P1_G
		mov		[esi-4],edx

		lea		esi,MODIFY_FOR_TEXTUREPOINTER_H
		mov		[esi-4],eax
		lea		esi,MODIFY_FOR_TEXTUREPOINTER_P1_H
		mov		[esi-4],ecx
		lea		esi,MODIFY_FOR_CLUT_H
		mov		[esi-4],ebx
		lea		esi,MODIFY_FOR_CLUT_P1_H
		mov		[esi-4],edx

DONE_WITH_MODIFY:
		//
		// Local copies of edge stepping values.
		//
		// fx_inc  = pdtri->pedgeBase->lineIncrement.fxX;
		// fx_diff = pdtri->pedgeBase->lineIncrement.fxXDifference;
		// f_inc_uinvz = pdtri->pedgeBase->lineIncrement.indCoord.fUInvZ;
		// f_inc_vinvz = pdtri->pedgeBase->lineIncrement.indCoord.fVInvZ;
		// f_inc_invz  = pdtri->pedgeBase->lineIncrement.indCoord.fInvZ;
		//
		mov		eax,[plinc]

		mov		esi,[pdtri]							// Pointer to polygon object.
		mov		ecx,[pscan]							// Pointer to scanline object.

		mov		ebx,[eax]TWaterPersp.fxX
		mov		edx,[eax]TWaterPersp.fxXDifference

		mov		fx_inc,ebx
		mov		fx_diff,edx

		mov		ebx,[eax]TWaterPersp.indCoord.fUInvZ
		mov		edx,[eax]TWaterPersp.indCoord.fVInvZ

		mov		f_inc_uinvz,ebx
		mov		ebx,[eax]TWaterPersp.indCoord.fInvZ

		mov		f_inc_vinvz,edx
		mov		f_inc_invz,ebx

		movzx	ebx,[u2WaterMask]

		mov		[u4WaterMask],ebx
		mov		edx,ebx

		shl		ebx,16

		or		ebx,edx

		mov		[u4DupWaterMask],ebx

		//
		// Iterate through the scanlines that intersect the subtriangle.
		//
Y_LOOP:
		push	ebp									// Save ebp.

		// Check to see if we sould skip this scanline.
		mov		eax,[esi]CDrawPolygonBase.iY
		mov		ebx,[bEvenScanlinesOnly]

		and		eax,ebx
		jnz		END_OF_SCANLINE

		// Setup for prespective correction and looping.
		PERSPECTIVE_SCANLINE_PROLOGUE_CLAMP(TWaterPersp)

		//
		// eax = i_pixel
		// edi = inner loop count
		// ecx,edx,esi = texture values
		//
		mov		ebp,gsGlobals.pvScreen				// Pointer the screen.
		mov		ebx,[i_screen_index]				// Load scanline offset.

		add		ebx,eax								// Add scanline offset to i_pixel
		mov		[i_pixel],eax						// Save i_pixel.

		lea		ebp,[ebp+ebx*2]						// Base of span in ebp.

		mov		[pvBaseOfLine],ebp					// Keep pointer to base of span.
		xor		eax,eax								// Clear eax.

		//-----------------------------------------------------------------------
		// Left to right scanline.
		lea		ebx,[ebp + edi*2]					// Calculate destination address.

		// Check alignement.
		and		ebx,3
	 	jz		short ALIGNED

		inc		edi
		jz		short FINISH_REMAINDER

		//
		// Do one pixel for alignment.
		//
		mov		al,[edx*2 + 0xDEADBEEF]				// Read texture value (low byte).
MODIFY_FOR_TEXTUREPOINTER_E:
		mov		bl,[ebp + edi*2 - 2]				// Read screen pixel (low byte).

		mov		ah,[edx*2 + 0xDEADBEEF]				// Read texture value (high byte).
MODIFY_FOR_TEXTUREPOINTER_P1_E:
		mov		bh,[ebp + edi*2 - 1]				// Read screen pixel (high byte).

		and		ebx,[u4WaterMask]					// Mask for alpha.
		add		ecx,[w2dDeltaTex]CWalk2D.uVFrac		// Step V fraction.

		sbb		ebp,ebp								// Get borrow from V fraction step.
		or		eax,ebx								// Combine alpha & masked color.

		add		esi,[w2dDeltaTex]CWalk2D.uUFrac				// Step U fraction.
		mov		ebp,[w2dDeltaTex + ebp*4 + 4]CWalk2D.iUVInt	// Integral step with v carry.

		adc		edx,ebp								// Integral step with u and v carry.
		mov		bl,[eax*2 + 0xDEADBEEF]				// Do CLUT lookup (low byte).
MODIFY_FOR_CLUT_E:

		mov		ebp,[pvBaseOfLine]					// Load pointer to destination.
		mov		bh,[eax*2 + 0xDEADBEEF]				// Do CLUT lookup (high byte).
MODIFY_FOR_CLUT_P1_E:

		mov		[ebp + edi*2 - 2],bx				// Store pixel.

ALIGNED:
		// Make sure we have two pixels left.
		add		edi,2
		jg		short FINISH_REMAINDER

		//
		// Do two pixels at a time.
		//
		mov		eax,[w2dDeltaTex]CWalk2D.uVFrac		// Load V fraction step.

INNER_LOOP_2P:
		push	edi									// Save edi
		add		ecx,eax								// Step V fraction.

		sbb		ebx,ebx								// Get borrow from V fraction step.
		mov		al,[edx*2 + 0xDEADBEEF]				// Read texture value P1 (low byte).
MODIFY_FOR_TEXTUREPOINTER_F:

		mov		edi,[ebp + edi*2 - 4]				// Read desitation pixels (P2,P1).
		mov		ebp,[w2dDeltaTex]CWalk2D.uUFrac		// Load U fraction step.

		mov		ah,[edx*2 + 0xDEADBEEF]				// Read texture value P1 (high byte).
MODIFY_FOR_TEXTUREPOINTER_P1_F:
		add		esi,ebp								// Step U fraction.

		adc		edx,[w2dDeltaTex + ebx*4 + 4]CWalk2D.iUVInt		// Integral step with u and v carry.
		and		edi,[u4DupWaterMask]							// Mask for alpha.

		shl		eax,16								// Shift P1 up.
		mov		ebp,[w2dDeltaTex]CWalk2D.uVFrac		// Load V fraction step.

		mov		al,[edx*2 + 0xDEADBEEF]				// Read texture value P2 (low byte).
MODIFY_FOR_TEXTUREPOINTER_G:

		mov		ah,[edx*2 + 0xDEADBEEF]				// Read texture value P2 (high byte).
MODIFY_FOR_TEXTUREPOINTER_P1_G:

		rol		eax,16								// Reverse pixels (P2,P1).

		or		eax,edi								// Combine alpha & masked color.
		add		ecx,ebp								// Step V fraction.

		sbb		ebp,ebp								// Get borrow from V fraction step.
		mov		edi,eax								// Copy combined pixels.

		shr		edi,16								// Shift high word down (P2).
		and		eax,0x0000ffff						// Mask off high word. (P1)

		mov		ebp,[w2dDeltaTex + ebp*4 + 4]CWalk2D.iUVInt		// Integral step with v carry.
		add		esi,[w2dDeltaTex]CWalk2D.uUFrac					// Step U fraction.

		adc		edx,ebp								// Integral step with u and v carry.
		mov		bl,[edi*2 + 0xDEADBEEF]				// Do CLUT lookup P2 (low byte).
MODIFY_FOR_CLUT_F:

		mov		bh,[edi*2 + 0xDEADBEEF]				// Do CLUT lookup P2 (high byte).
MODIFY_FOR_CLUT_P1_F:
		pop		edi									// Restore edi.

		shl		ebx,16								// Shift pixel 2 up.
		mov		ebp,[pvBaseOfLine]					// Load pointer to destination.

		mov		bl,[eax*2 + 0xDEADBEEF]				// Do CLUT lookup P1 (low byte).
MODIFY_FOR_CLUT_G:
		add		edi,2								// Step desitination.

		mov		bh,[eax*2 + 0xDEADBEEF]				// Do CLUT lookup P1 (high byte).
MODIFY_FOR_CLUT_P1_G:
		mov		eax,[w2dDeltaTex]CWalk2D.uVFrac		// Load V fraction step.

		mov		[ebp + edi*2 - 8],ebx				// Store pixels.
		jle		short INNER_LOOP_2P

		// edi is 1 if there is a pixel left.
		cmp		edi,1
		jne		short DONE_WITH_SPAN

		//
		// Finish left over pixel.
		//
		// ebp points to base of line.
		//
FINISH_REMAINDER:
		xor		eax,eax								// Clear eax.

		mov		al,[edx*2 + 0xDEADBEEF]				// Read texture value (low byte).
MODIFY_FOR_TEXTUREPOINTER_H:
		mov		bl,[ebp + edi*2 - 2]				// Read screen pixel (low byte).

		mov		ah,[edx*2 + 0xDEADBEEF]				// Read texture value (high byte).
MODIFY_FOR_TEXTUREPOINTER_P1_H:
		mov		bh,[ebp + edi*2 - 1]				// Read screen pixel (high byte).

		and		ebx,[u4WaterMask]					// Mask for alpha.
		add		ecx,[w2dDeltaTex]CWalk2D.uVFrac		// Step V fraction.

		sbb		ebp,ebp								// Get borrow from V fraction step.
		or		eax,ebx								// Combine alpha & masked color.

		add		esi,[w2dDeltaTex]CWalk2D.uUFrac				// Step U fraction.
		mov		ebp,[w2dDeltaTex + ebp*4 + 4]CWalk2D.iUVInt	// Integral step with v carry.

		adc		edx,ebp								// Integral step with u and v carry.
		mov		bl,[eax*2 + 0xDEADBEEF]				// Do CLUT lookup (low byte).
MODIFY_FOR_CLUT_H:

		mov		ebp,[pvBaseOfLine]					// Load pointer to destination.
		mov		bh,[eax*2 + 0xDEADBEEF]				// Do CLUT lookup (high byte).
MODIFY_FOR_CLUT_P1_H:

		mov		[ebp - 2],bx						// Store pixel.

DONE_WITH_SPAN:
		// See if there are any pixels left.
		cmp		[i_pixel],0
		je		END_OF_SCANLINE

		// Do perspective correction and looping.
		PERSPECTIVE_SCANLINE_EPILOGUE_CLAMP

END_OF_SCANLINE:
		pop		ebp						// Restore ebp.

		//
		// Increment the base edge.
		//
		mov		esi,[pdtri]							// Pointer to polygon object.
		mov		ecx,[pscan]							// Pointer to scanline object.

		// Step length, line starting address.
		mov		eax,[esi]CDrawPolygonBase.fxLineLength.i4Fx
		mov		ebx,[esi]CDrawPolygonBase.iLineStartIndex

		add		eax,[esi]CDrawPolygonBase.fxDeltaLineLength.i4Fx
		add		ebx,[gsGlobals.u4LinePixels]

		mov		[esi]CDrawPolygonBase.fxLineLength.i4Fx,eax
		mov		[esi]CDrawPolygonBase.iLineStartIndex,ebx

		// Step edge values.
		mov		eax,[ecx]TWaterPersp.fxX.i4Fx
		mov		ebx,[ecx]TWaterPersp.fxXDifference.i4Fx

		fld		[ecx]TWaterPersp.indCoord.fUInvZ
		fadd	[f_inc_uinvz]
		fld		[ecx]TWaterPersp.indCoord.fVInvZ
		fadd	[f_inc_vinvz]
		fld		[ecx]TWaterPersp.indCoord.fInvZ
		fadd	[f_inc_invz]

		add		eax,[fx_inc]
		add		ebx,[fx_diff]

		jge		short NO_UNDERFLOW

		// Underflow.
		add		ebx,0x00010000
		jmp		short BORROW_OR_CARRY

NO_UNDERFLOW:
		cmp		ebx,0x00010000
		jle		short NO_OVERFLOW

		// Overflow.
		sub		ebx,0x00010000
		jmp		short BORROW_OR_CARRY

NO_OVERFLOW:
		fstp	[ecx]TWaterPersp.indCoord.fInvZ
		fstp	[ecx]TWaterPersp.indCoord.fVInvZ
		fstp	[ecx]TWaterPersp.indCoord.fUInvZ

		jmp		short FINISH_LOOPING

BORROW_OR_CARRY:
		fadd	[fNegInvZ]								// Z V U
		fxch	st(1)									// V Z U
		fadd	[fNegVInvZ]								// V Z U
		fxch	st(2)									// U Z V
		fadd	[fNegUInvZ]								// U Z V
		fxch	st(1)									// Z U V
		fstp	[ecx]TWaterPersp.indCoord.fInvZ
		fstp	[ecx]TWaterPersp.indCoord.fUInvZ
		fstp	[ecx]TWaterPersp.indCoord.fVInvZ

FINISH_LOOPING:
		// Y Loop control.
		mov		[ecx]TWaterPersp.fxX.i4Fx,eax
		mov		edx,[esi]CDrawPolygonBase.iY

		mov		[ecx]TWaterPersp.fxXDifference.i4Fx,ebx
		inc		edx

		mov		ebx,[esi]CDrawPolygonBase.iYTo
		mov		[esi]CDrawPolygonBase.iY,edx

		cmp		edx,ebx
		jl		Y_LOOP
	}
}


//*****************************************************************************************
//
// Linear, alpha textured, 16 bit mode.
//
void DrawSubtriangle(TWaterLinear* pscan, CDrawPolygon<TWaterLinear>* pdtri)
{
	typedef TWaterLinear::TIndex prevent_internal_compiler_errors;
	static void* pvLastClut = 0;
	static void* pvLastTexture = 0;
	static CDrawPolygon<TWaterLinear>* pdtriGlbl;
	static TWaterLinear* pscanGlbl;

	TWaterLinear* plinc = &pdtri->pedgeBase->lineIncrement;

	Assert(pu2WaterAlpha);

	__asm
	{
		//
		// Do self modification.
		//
		mov		ecx,[pvTextureBitmap]				// Texture pointer.
		mov		edx,[pu2WaterAlpha]					// Clut conversion pointer.
		
		mov		eax,[pvLastTexture]
		mov		ebx,[pvLastClut]

		cmp		eax,ecx
		jne		short DO_MODIFY

		cmp		ebx,edx
		je		short DONE_WITH_MODIFY

DO_MODIFY:
		mov		[pvLastTexture],ecx					// Update last texture pointer.
		mov		[pvLastClut],edx					// Update last clut pointer.

		mov	eax,ecx									// Texture pointer plus one.
		inc	ecx
		mov	ebx,edx									// Clut pointer plus one.
		inc	edx

		lea		esi,MODIFY_FOR_TEXTUREPOINTER_E
		mov		[esi-4],eax
		lea		esi,MODIFY_FOR_TEXTUREPOINTER_P1_E
		mov		[esi-4],ecx
		lea		esi,MODIFY_FOR_CLUT_E
		mov		[esi-4],ebx
		lea		esi,MODIFY_FOR_CLUT_P1_E
		mov		[esi-4],edx

		lea		esi,MODIFY_FOR_TEXTUREPOINTER_F
		mov		[esi-4],eax
		lea		esi,MODIFY_FOR_TEXTUREPOINTER_P1_F
		mov		[esi-4],ecx
		lea		esi,MODIFY_FOR_CLUT_F
		mov		[esi-4],ebx
		lea		esi,MODIFY_FOR_CLUT_P1_F
		mov		[esi-4],edx

		lea		esi,MODIFY_FOR_TEXTUREPOINTER_G
		mov		[esi-4],eax
		lea		esi,MODIFY_FOR_TEXTUREPOINTER_P1_G
		mov		[esi-4],ecx
		lea		esi,MODIFY_FOR_CLUT_G
		mov		[esi-4],ebx
		lea		esi,MODIFY_FOR_CLUT_P1_G
		mov		[esi-4],edx

		lea		esi,MODIFY_FOR_TEXTUREPOINTER_H
		mov		[esi-4],eax
		lea		esi,MODIFY_FOR_TEXTUREPOINTER_P1_H
		mov		[esi-4],ecx
		lea		esi,MODIFY_FOR_CLUT_H
		mov		[esi-4],ebx
		lea		esi,MODIFY_FOR_CLUT_P1_H
		mov		[esi-4],edx

DONE_WITH_MODIFY:
		push	ebp									// Save ebp.
		mov		edi,[plinc]							// Pointer to scanline increment.

		mov		eax,[pscan]							// Pointer to scanline.
		mov		esi,[pdtri]							// Pointer to polygon.

		//
		// Copies of edge stepping values.
		//
		// fx_inc			= pdtri->pedgeBase->lineIncrement.fxX;
		// fx_diff			= pdtri->pedgeBase->lineIncrement.fxXDifference;
		// bf_u_inc			= pdtri->pedgeBase->lineIncrement.indCoord.bfU;
		// w1d_v_inc		= pdtri->pedgeBase->lineIncrement.indCoord.w1dV;
		// pscanGlbl		= pscan;
		// pdtriGlbl		= pdtri;
		//
		mov		ebx,[edi]TWaterLinear.fxX
		mov		ecx,[edi]TWaterLinear.fxXDifference

		mov		fx_inc,ebx
		mov		fx_diff,ecx

		mov		ebx,[edi]TWaterLinear.indCoord.bfU.i4Int
		mov		ecx,[edi]TWaterLinear.indCoord.bfU.u4Frac

		mov		bf_u_inc.i4Int,ebx
		mov		bf_u_inc.u4Frac,ecx

		mov		ebx,[edi]TWaterLinear.indCoord.w1dV.bfxValue.i4Int
		mov		ecx,[edi]TWaterLinear.indCoord.w1dV.bfxValue.u4Frac

		mov		w1d_v_inc.bfxValue.i4Int,ebx
		mov		w1d_v_inc.bfxValue.u4Frac,ecx

		mov		pdtriGlbl,esi
		mov		pscanGlbl,eax

		movzx	ebx,[u2WaterMask]

		mov		[u4WaterMask],ebx
		mov		edx,ebx

		shl		ebx,16

		or		ebx,edx

		mov		[u4DupWaterMask],ebx

		//
		// Iterate through the scanlines that intersect the subtriangle.
		//
Y_LOOP:
		// Check to see if we should skip this scanline.
		mov		ecx,[esi]CDrawPolygonBase.iY
		mov		ebx,[bEvenScanlinesOnly]

		and		ecx,ebx
		jnz		END_OF_SCANLINE

		// Determine the start and end of the scanline.
		mov		ebx,[eax]TWaterLinear.fxX.i4Fx
		mov		ecx,[esi]CDrawPolygonBase.fxLineLength.i4Fx

		mov		edx,[esi]CDrawPolygonBase.iLineStartIndex
		add		ebx,ecx

		sar		ebx,16
		mov		edi,[eax]TWaterLinear.fxX.i4Fx

		sar		edi,16
		add		edx,ebx

		sub		edi,ebx									// i_pixel
		jge		END_OF_SCANLINE

		//
		// edi = i_pixel
		// edx = i_screen_index
		// eax = pointer to scanline
		//
		mov		ebp,gsGlobals.pvScreen									// Pointer the screen.
		mov		ebx,[eax]TWaterLinear.indCoord.bfU.i4Int				// Uint

		mov		esi,[eax]TWaterLinear.indCoord.bfU.u4Frac				// Ufrac
		mov		ecx,[eax]TWaterLinear.indCoord.w1dV.bfxValue.u4Frac		// Vfrac

		lea		ebp,[ebp+edx*2]											// Base of scanline.
		mov		edx,[eax]TWaterLinear.indCoord.w1dV.bfxValue.i4Int		// Vint

		add		edx,ebx													// Add Uint to Vint

		mov		[pvBaseOfLine],ebp					// Save pointer to base of line.

		//-----------------------------------------------------------------------
		// Left to right scanline.
		lea		ebx,[ebp + edi*2]					// Calculate destination address.
		xor		eax,eax								// Clear eax.

		// Check alignement.
		and		ebx,3
		jz		short ALIGNED

		// Check for one pixel before starting.
		inc		edi
		jz		short FINISH_REMAINDER

		//
		// Do one pixel for alignment.
		//
		mov		al,[edx*2 + 0xDEADBEEF]				// Read texture value (low byte).
MODIFY_FOR_TEXTUREPOINTER_E:
		mov		bl,[ebp + edi*2 - 2]				// Read screen pixel (low byte).

		mov		ah,[edx*2 + 0xDEADBEEF]				// Read texture value (high byte).
MODIFY_FOR_TEXTUREPOINTER_P1_E:
		mov		bh,[ebp + edi*2 - 1]				// Read screen pixel (high byte).

		and		ebx,[u4WaterMask]					// Mask for alpha.
		add		ecx,[w2dDeltaTex]CWalk2D.uVFrac		// Step V fraction.

		sbb		ebp,ebp								// Get borrow from V fraction step.
		or		eax,ebx								// Combine alpha & masked color.

		add		esi,[w2dDeltaTex]CWalk2D.uUFrac				// Step U fraction.
		mov		ebp,[w2dDeltaTex + ebp*4 + 4]CWalk2D.iUVInt	// Integral step with v carry.

		adc		edx,ebp								// Integral step with u and v carry.
		mov		bl,[eax*2 + 0xDEADBEEF]				// Do CLUT lookup (low byte).
MODIFY_FOR_CLUT_E:

		mov		ebp,[pvBaseOfLine]					// Load pointer to destination.
		mov		bh,[eax*2 + 0xDEADBEEF]				// Do CLUT lookup (high byte).
MODIFY_FOR_CLUT_P1_E:

		mov		[ebp + edi*2 - 2],bx				// Store pixel.

ALIGNED:
		// Make sure we have two pixels left.
		add		edi,2
		jg		short FINISH_REMAINDER

		//
		// Do two pixels at a time.
		//
		mov		eax,[w2dDeltaTex]CWalk2D.uVFrac		// Load V fraction step.

INNER_LOOP_2P:
		push	edi									// Save edi
		add		ecx,eax								// Step V fraction.

		sbb		ebx,ebx								// Get borrow from V fraction step.
		mov		al,[edx*2 + 0xDEADBEEF]				// Read texture value P1 (low byte).
MODIFY_FOR_TEXTUREPOINTER_F:

		mov		edi,[ebp + edi*2 - 4]				// Read desitation pixels (P2,P1).
		mov		ebp,[w2dDeltaTex]CWalk2D.uUFrac		// Load U fraction step.

		mov		ah,[edx*2 + 0xDEADBEEF]				// Read texture value P1 (high byte).
MODIFY_FOR_TEXTUREPOINTER_P1_F:
		add		esi,ebp								// Step U fraction.

		adc		edx,[w2dDeltaTex + ebx*4 + 4]CWalk2D.iUVInt		// Integral step with u and v carry.
		and		edi,[u4DupWaterMask]							// Mask for alpha.

		shl		eax,16								// Shift P1 up.
		mov		ebp,[w2dDeltaTex]CWalk2D.uVFrac		// Load V fraction step.

		mov		al,[edx*2 + 0xDEADBEEF]				// Read texture value P2 (low byte).
MODIFY_FOR_TEXTUREPOINTER_G:

		mov		ah,[edx*2 + 0xDEADBEEF]				// Read texture value P2 (high byte).
MODIFY_FOR_TEXTUREPOINTER_P1_G:

		rol		eax,16								// Reverse pixels (P2,P1).

		or		eax,edi								// Combine alpha & masked color.
		add		ecx,ebp								// Step V fraction.

		sbb		ebp,ebp								// Get borrow from V fraction step.
		mov		edi,eax								// Copy combined pixels.

		shr		edi,16								// Shift high word down (P2).
		and		eax,0x0000ffff						// Mask off high word. (P1)

		mov		ebp,[w2dDeltaTex + ebp*4 + 4]CWalk2D.iUVInt		// Integral step with v carry.
		add		esi,[w2dDeltaTex]CWalk2D.uUFrac					// Step U fraction.

		adc		edx,ebp								// Integral step with u and v carry.
		mov		bl,[edi*2 + 0xDEADBEEF]				// Do CLUT lookup P2 (low byte).
MODIFY_FOR_CLUT_F:

		mov		bh,[edi*2 + 0xDEADBEEF]				// Do CLUT lookup P2 (high byte).
MODIFY_FOR_CLUT_P1_F:
		pop		edi									// Restore edi.

		shl		ebx,16								// Shift pixel 2 up.
		mov		ebp,[pvBaseOfLine]					// Load pointer to destination.

		mov		bl,[eax*2 + 0xDEADBEEF]				// Do CLUT lookup P1 (low byte).
MODIFY_FOR_CLUT_G:
		add		edi,2								// Step desitination.

		mov		bh,[eax*2 + 0xDEADBEEF]				// Do CLUT lookup P1 (high byte).
MODIFY_FOR_CLUT_P1_G:
		mov		eax,[w2dDeltaTex]CWalk2D.uVFrac		// Load V fraction step.

		mov		[ebp + edi*2 - 8],ebx				// Store pixels.
		jle		short INNER_LOOP_2P

		// edi is 1 if there is a pixel left.
		cmp		edi,1
		jne		short END_OF_SCANLINE

		//
		// Finish left over pixel.
		//
		// eax is zero.
		// ebp points to base of line.
		//
FINISH_REMAINDER:
		xor		eax,eax								// Clear eax.

		mov		al,[edx*2 + 0xDEADBEEF]				// Read texture value (low byte).
MODIFY_FOR_TEXTUREPOINTER_H:
		mov		bl,[ebp - 2]						// Read screen pixel (low byte).

		mov		ah,[edx*2 + 0xDEADBEEF]				// Read texture value (high byte).
MODIFY_FOR_TEXTUREPOINTER_P1_H:
		mov		bh,[ebp - 1]						// Read screen pixel (high byte).

		and		ebx,[u4WaterMask]					// Mask for alpha.

		or		eax,ebx								// Combine alpha & masked color.

		mov		bl,[eax*2 + 0xDEADBEEF]				// Do CLUT lookup (low byte).
MODIFY_FOR_CLUT_H:

		mov		bh,[eax*2 + 0xDEADBEEF]				// Do CLUT lookup (high byte).
MODIFY_FOR_CLUT_P1_H:

		mov		[ebp - 2],bx						// Store pixel.

END_OF_SCANLINE:
		mov		eax,[pscanGlbl]
		mov		ebp,bf_u_inc.i4Int

		// AGI on eax.

		mov		ecx,[eax]TWaterLinear.indCoord.bfU.u4Frac
		mov		edx,[eax]TWaterLinear.indCoord.bfU.i4Int

		mov		edi,[eax]TWaterLinear.indCoord.w1dV.bfxValue.i4Int
		mov		esi,[eax]TWaterLinear.indCoord.w1dV.bfxValue.u4Frac

		add		edi,w1d_v_inc.bfxValue.i4Int
		add		ecx,bf_u_inc.u4Frac

		adc		edx,ebp
		mov		ebp,w1d_v_inc.bfxValue.u4Frac

		add		esi,ebp
		jnc		short NO_VFRAC_CARRY

		add		edi,[eax]TWaterLinear.indCoord.w1dV.iOffsetPerLine

NO_VFRAC_CARRY:
		mov		ebx,[eax]TWaterLinear.fxXDifference
		mov		ebp,[fx_diff]

		add		ebx,ebp
		jge		short NO_UNDERFLOW

		// Undeflow of fxXDifference.
		add		ebx,0x00010000
		mov		ebp,bfNegDU.i4Int

		add		edi,w1dNegDV.bfxValue.i4Int
		add		ecx,bfNegDU.u4Frac

		adc		edx,ebp
		mov		ebp,w1dNegDV.bfxValue.u4Frac

		add		esi,ebp
		jnc		short EDGE_DONE

		add		edi,[eax]TWaterLinear.indCoord.w1dV.iOffsetPerLine
		jmp		short EDGE_DONE

NO_UNDERFLOW:
		cmp		ebx,0x00010000
		jl		short EDGE_DONE

		// Overflow of fxXDifference.
		sub		ebx,0x00010000
		mov		ebp,bfNegDU.i4Int

		add		edi,w1dNegDV.bfxValue.i4Int
		add		ecx,bfNegDU.u4Frac

		adc		edx,ebp
		mov		ebp,w1dNegDV.bfxValue.u4Frac

		add		esi,ebp
		jnc		short EDGE_DONE

		add		edi,[eax]TWaterLinear.indCoord.w1dV.iOffsetPerLine

EDGE_DONE:
		// Store modified variables and do looping.
		mov		[eax]TWaterLinear.indCoord.w1dV.bfxValue.u4Frac,esi
		mov		[eax]TWaterLinear.indCoord.w1dV.bfxValue.i4Int,edi

		mov		[eax]TWaterLinear.fxXDifference,ebx
		mov		esi,[pdtriGlbl]								// Pointer to polygon object.

		mov		[eax]TWaterLinear.indCoord.bfU.u4Frac,ecx
		mov		[eax]TWaterLinear.indCoord.bfU.i4Int,edx

		// Step length, line starting address.
		mov		ebx,[esi]CDrawPolygonBase.fxLineLength.i4Fx
		mov		ecx,[esi]CDrawPolygonBase.iLineStartIndex

		add		ebx,[esi]CDrawPolygonBase.fxDeltaLineLength.i4Fx
		add		ecx,[gsGlobals.u4LinePixels]

		mov		[esi]CDrawPolygonBase.fxLineLength.i4Fx,ebx
		mov		[esi]CDrawPolygonBase.iLineStartIndex,ecx

		mov		ecx,[eax]TWaterLinear.fxX
		mov		ebp,[fx_inc]

		add		ecx,ebp
		mov		edx,[esi]CDrawPolygonBase.iY

		mov		[eax]TWaterLinear.fxX,ecx
		inc		edx

		mov		ebx,[esi]CDrawPolygonBase.iYTo
		mov		[esi]CDrawPolygonBase.iY,edx

		cmp		edx,ebx
		jl		Y_LOOP

		pop ebp
	}
}


//*****************************************************************************************
//
// Perspective, stipple textured, 16 bit mode.
//
void DrawSubtriangle(TStippleTexPersp* pscan, CDrawPolygon<TStippleTexPersp>* pdtri)
{
	typedef TStippleTexPersp::TIndex prevent_internal_compiler_errors;
	static void* pvLastClut = 0;
	static void* pvLastTexture = 0;

	TStippleTexPersp* plinc = &pdtri->pedgeBase->lineIncrement;

	__asm
	{
		//
		// Local copies of edge stepping values.
		//
		// fx_inc  = pdtri->pedgeBase->lineIncrement.fxX;
		// fx_diff = pdtri->pedgeBase->lineIncrement.fxXDifference;
		// f_inc_uinvz = pdtri->pedgeBase->lineIncrement.indCoord.fUInvZ;
		// f_inc_vinvz = pdtri->pedgeBase->lineIncrement.indCoord.fVInvZ;
		// f_inc_invz  = pdtri->pedgeBase->lineIncrement.indCoord.fInvZ;
		//
		mov		eax,[plinc]

		mov		esi,[pdtri]							// Pointer to polygon object.
		mov		ecx,[pscan]							// Pointer to scanline object.

		mov		ebx,[eax]TStippleTexPersp.fxX
		mov		edx,[eax]TStippleTexPersp.fxXDifference

		mov		fx_inc,ebx
		mov		fx_diff,edx

		mov		ebx,[eax]TStippleTexPersp.indCoord.fUInvZ
		mov		edx,[eax]TStippleTexPersp.indCoord.fVInvZ

		mov		f_inc_uinvz,ebx
		mov		ebx,[eax]TStippleTexPersp.indCoord.fInvZ

		mov		f_inc_vinvz,edx
		mov		f_inc_invz,ebx

		//
		// Iterate through the scanlines that intersect the subtriangle.
		//
Y_LOOP:
		push	ebp									// Save ebp.

		// Check to see if we sould skip this scanline.
		mov		eax,[esi]CDrawPolygonBase.iY
		mov		ebx,[bEvenScanlinesOnly]

		and		eax,ebx
		jnz		END_OF_SCANLINE

		// Setup for prespective correction and looping.
		//PERSPECTIVE_SCANLINE_PROLOGUE_CLAMP(TStippleTexPersp)

		// Determine the start and end of the scanline. 
		mov		ebx,[ecx]TStippleTexPersp.fxX.i4Fx
		mov		eax,[esi]CDrawPolygonBase.fxLineLength.i4Fx

		add		ebx,eax
		mov		eax,[ecx]TStippleTexPersp.fxX.i4Fx

		sar		ebx,16
		mov		edx,[esi]CDrawPolygonBase.iLineStartIndex

		sar		eax,16
		add		edx,ebx

		sub		eax,ebx									// i_pixel 
		jge		END_OF_SCANLINE

		mov		[i_screen_index],edx
		mov		ebx,[ecx]TStippleTexPersp.fxX.i4Fx		// Fractional X.

		sar		ebx,16									// Integer X.
		mov		edi,[esi]CDrawPolygonBase.iY			// Integer Y.

		xor		ebx,edi									// X xor Y
		mov		[i_pixel],eax

		// Skip over a pixel if ((y xor x) & 1).
		test	ebx,1
		jz		NO_SKIP

		inc		eax										// Increment i_pixel
		jz		END_OF_SCANLINE

		// Copy and increment global texture values (16 cycles).
		fld		fDUInvZEdge								// ustep
		fmul	fInvSubdivideLen
		fld		fDVInvZEdge								// vstep
		fmul	fInvSubdivideLen
		fld		fDInvZEdge								// zstep
		fmul	fInvSubdivideLen
		fxch	st(2)									// us vs zs
		fadd	[ecx]TStippleTexPersp.indCoord.fUInvZ	// u vs zs
		fxch	st(1)
		fadd	[ecx]TStippleTexPersp.indCoord.fVInvZ	// v u zs
		fxch	st(2)
		fadd	[ecx]TStippleTexPersp.indCoord.fInvZ	// z u v
		fxch	st(1)									// u z v
		fstp	fGUInvZ
		fxch	st(1)
		fstp	fGVInvZ
		fstp	fGInvZ

		mov		[i_pixel],eax
		jmp		short DONE_WITH_SKIP

		//--------------------------------------------------------------------------- 
		// Caclulate values for the first correction span, and start the divide for 
		// the next span. 
		// 
NO_SKIP:
		// Copy global texture values. 
		mov		ebx,[ecx]TStippleTexPersp.indCoord.fUInvZ
		mov		esi,[ecx]TStippleTexPersp.indCoord.fVInvZ

		mov		edi,[ecx]TStippleTexPersp.indCoord.fInvZ
		mov		fGUInvZ,ebx

		mov		fGVInvZ,esi
		mov		fGInvZ,edi

DONE_WITH_SKIP:
		// Copy edge values.
		mov		ebx,fDUInvZEdge
		mov		esi,fDVInvZEdge

		mov		edi,fDInvZEdge
		mov		fDUInvZScanline,ebx

		mov		fDVInvZScanline,esi
		mov		fDInvZScanline,edi

		mov		ebx,[iSubdivideLen]

		// scan line is +ve 
		add		eax,ebx
		jle		short DONE_DIVIDE_PIXEL

		// calc the new +ve ratio 
		fild	[i_pixel]
		fld		fInvSubdivideLen
		fchs

		// Subdivision is smaller than iSubdivideLen 
		fmulp	st(1),st(0)			// st(0) = (-)fInvSubdivideLen * i_pixel; 
		fld		fDUInvZScanline		// U C 
		xor		eax,eax
		fmul	st(0),st(1)			// U*C C 
		fxch	st(1)				// C U*C 
		fld		fDVInvZScanline		// V C U*C 
		fxch	st(1)				// C V U*C 
		fmul	st(1),st(0)			// C V*C U*C 
		// stall(1) 
		fmul	fDInvZScanline		// Z*C V*C U*C 
		fxch	st(2)				// U*C V*C Z*C 
		fstp	fDUInvZScanline		// V*C Z*C 
		fstp	fDVInvZScanline		// Z*C 
		fstp	fDInvZScanline

DONE_DIVIDE_PIXEL:
		// Get current u, v and z values. 
		mov		[iNextSubdivide],eax
		mov		ebx,dword ptr[fGInvZ]		// f_z = fInverse(fGInvZ); 

		mov		eax,iFI_SIGN_EXPONENT_SUB

		sub		eax,ebx
		and		ebx,iFI_MASK_MANTISSA

		sar		ebx,iSHIFT_MANTISSA
		and		eax,iFI_MASK_SIGN_EXPONENT

		fld		[fGUInvZ]					// U/Z 

		add		eax,dword ptr[i4InverseMantissa + ebx*4]

		mov		dword ptr[f_z],eax

		//--------------------------------------------------------------------------- 
		// Set current texture coordinates (clamped). 
		fmul	[f_z]
		fld		[fGVInvZ]
		fmul	[f_z]
		fxch	st(1)
		// stall(1) 
		fstp	[f_u]

		// Clamp f_u			 
		mov		eax,[f_u]
		mov		ebx,fTexEdgeTolerance
		mov		ecx,fTexWidth
		cmp		eax,ebx
		jge		short U_NOT_LESS1
		mov		eax,ebx
U_NOT_LESS1:
		cmp		eax,ecx
		jle		short U_NOT_GREATER1
		mov		eax,ecx
U_NOT_GREATER1:
		mov		ecx,fTexHeight
		mov		[f_u],eax
		fld		[f_u]

		fadd	[dFastFixed16Conversion]
		fxch	st(1)

		// Clamp f_v 
		fstp	[f_v]
		mov		eax,[f_v]
		cmp		eax,ebx
		jge		short V_NOT_LESS1
		mov		eax,ebx
V_NOT_LESS1:
		cmp		eax,ecx
		jle		short V_NOT_GREATER1
		mov		eax,ecx
V_NOT_GREATER1:
		mov		[f_v],eax
		fld		[f_v]

		fadd	[dFastFixed16Conversion]
		fxch	st(1)
		fstp	[d_temp_a]
		fld		[fGUInvZ]
		fxch	st(1)
		fstp	[d_temp_b]

		// Increment u, v and z values. 
		fadd	[fDUInvZScanline]
		fld		[fGVInvZ]
		fadd	[fDVInvZScanline]
		fxch	st(1)
		fld		[fGInvZ]
		fadd	[fDInvZScanline]

		// Setup esi=uFrac, ecx=vFrac, edx=UVInt for Abrash texture loop. 
		mov		edx,dword ptr[d_temp_a]			// U (16.16) 
		mov		eax,dword ptr[d_temp_b]			// V (16.16) 

		sar		eax,16							// Integral V 
		mov		ecx,[iTexWidth]					// Texture width. 

		imul	eax,ecx							// iv*twidth 

		sar		edx,16							// Integral U 
		mov		esi,dword ptr[d_temp_a]			// Copy of U 

		shl		esi,16							// UFrac 
		mov		ecx,dword ptr[d_temp_b]			// Copy of V 

		shl		ecx,16							// VFrac 
		mov		ebx,[pvTextureBitmap]			// Texture base pointer.

		shr		ebx,1							// Turn into word offset.
		add		edx,eax							// iu + iv*twidth 

		add		edx,ebx							// Add to edx

		fstp	[fGInvZ]

		// Get next u, v and z values. 
		mov		ebx,dword ptr[fGInvZ]			// f_next_z = fInverse(fGInvZ); 
		mov		eax,iFI_SIGN_EXPONENT_SUB

		sub		eax,ebx
		and		ebx,iFI_MASK_MANTISSA

		sar		ebx,iSHIFT_MANTISSA
		and		eax,iFI_MASK_SIGN_EXPONENT

		fst		[fGUInvZ]

		mov		ebx,dword ptr[i4InverseMantissa + ebx*4]
		mov		edi,[i_pixel]

		add		eax,ebx
		mov		ebx,[iNextSubdivide]

		mov		dword ptr[f_next_z],eax
		sub		edi,ebx

		// Set new texture coordinate increments. 
		fmul	[f_next_z]
		fxch	st(1)
		fst		[fGVInvZ]
		fmul	[f_next_z]
		fxch	st(1)
		fstp	[fU]			// V 

		// Clamp fU			 
		mov		eax,[fU]
		mov		ebx,fTexEdgeTolerance
		mov		ebp,fTexWidth
		cmp		eax,ebx
		jge		short U_NOT_LESS2
		mov		eax,ebx
U_NOT_LESS2:
		cmp		eax,ebp
		jle		short U_NOT_GREATER2
		mov		eax,ebp
U_NOT_GREATER2:
		mov		ebp,fTexHeight
		mov		[fU],eax

		fld		[fU]							// U,V 
		fsub	[f_u]							// U-fu,V 
		fxch	st(1)							// V,U-fu 
		fstp	[fV]							// U-fu 

		// Clamp fV 
		mov		eax,[fV]
		cmp		eax,ebx
		jge		short V_NOT_LESS2
		mov		eax,ebx
V_NOT_LESS2:
		cmp		eax,ebp
		jle		short V_NOT_GREATER2
		mov		eax,ebp
V_NOT_GREATER2:
		mov		[fV],eax

		fld		[fV]								// V U-fu 
		fsub	[f_v]								// V-fv,U-fu 
		fxch	st(1)								// U,V 
		fmul	float ptr[fInverseIntTable+edi*4]	// (C*U),V 
		fxch	st(1)								// V,(C*U) 
		// stall(1) 
		fmul	float ptr[fInverseIntTable+edi*4]	// (C*V),(C*U) 
		fxch	st(1)								// (C*U),(C*V) 
		fadd	[dFastFixed16Conversion]
		fxch	st(1)
		// stall(1) 
		fadd	[dFastFixed16Conversion]
		fxch	st(1)
		// stall(1) 
		fstp	[d_temp_a]
		fstp	[d_temp_b]

		mov		edi,dword ptr[d_temp_a]			// uslope 
		mov		eax,dword ptr[d_temp_b]			// vslope 

		sar		edi,15							// integer part of uslope 
		mov		ebp,dword ptr[d_temp_a]			// uslope again 

		shl		ebp,17							// fractional part of uslope 
		mov		ebx,eax							// vslope again 

		sar		eax,15							// integer part of vslope 
		mov		[w2dDeltaTex.uUFrac],ebp		// store UFrac 

		imul	eax,[iTexWidth]					// ivslope*twidth 

		shl		ebx,17							// fractional part of vslope 

		mov		[w2dDeltaTex.uVFrac],ebx		// store VFrac 

		add		edi,eax							// ivslope*twidth + iuslope 
		mov		ebp,[iTexWidth]					// Load texture width. 

		mov		[w2dDeltaTex.iUVInt+4],edi		// store integer stride 
		add		edi,ebp							// add twidth to integer stride 

		mov		[w2dDeltaTex.iUVInt],edi		// store integer stride + twidth 
		mov		eax,[iNextSubdivide]

		test	eax,eax							// Next subdivision is zero length? 
		jz		SUBDIVISION_LOOP

		// iCacheNextSubdivide = iSetNextDividePixel(iNextSubdivide); 
		mov		edi,[iSubdivideLen]

		// scan line is +ve 
		add		eax,edi
		jle		short DONE_DIVIDE_PIXEL_CACHE

		// calc the new +ve ratio 
		fild	iNextSubdivide
		fld		fInvSubdivideLen
		fchs

		// Subdivision is smaller than iSubdivideLen 
		fmulp	st(1),st(0)			// st(0) = (-)fInvSubdivideLen * i_pixel; 
		fld		fDUInvZEdge			// U C 

		xor		eax,eax

		fmul	st(0),st(1)			// U*C C 
		fxch	st(1)				// C U*C 
		fld		fDVInvZEdge			// V C U*C 
		fxch	st(1)				// C V U*C 
		fmul	st(1),st(0)			// C V*C U*C 

		mov		[iCacheNextSubdivide],eax

		fmul	fDInvZEdge			// Z*C V*C U*C 
		fxch	st(2)				// U*C V*C Z*C 
		fst		fDUInvZScanline
		fadd	fGUInvZ
		fxch	st(1)
		fst		fDVInvZScanline
		fadd	fGVInvZ
		fxch	st(2)
		fst		fDInvZScanline
		fadd	fGInvZ
		fxch	st(2)
		fstp	fGVInvZ			
		fstp	fGUInvZ
		fst		fGInvZ
		fdivr	fOne							// Start the next division. 

		jmp		SUBDIVISION_LOOP

DONE_DIVIDE_PIXEL_CACHE:
		// Copy texture values. 
		mov		ebp,[fDUInvZEdge]
		mov		ebx,[fDVInvZEdge]

		mov		edi,[fDInvZEdge]
		mov		[fDUInvZScanline],ebp

		mov		[fDVInvZScanline],ebx
		mov		[fDInvZScanline],edi

		fld		fDUInvZScanline			// U 
		fadd	fGUInvZ
		fld		fDVInvZScanline			// V U 
		fadd	fGVInvZ
		fld		fDInvZScanline			// Z V U 
		fadd	fGInvZ
		fxch	st(2)					// U V Z 
		fstp	fGUInvZ
		fstp	fGVInvZ
		fst		fGInvZ
		fdivr	fOne							// Start the next division. 

		mov		[iCacheNextSubdivide],eax

SUBDIVISION_LOOP:
		//--------------------------------------------------------------------------- 
		// Start the next subdivision. 
		// ecx,edx,esi = texture values 
		// 
		mov		edi,[i_pixel]
		mov		eax,[iNextSubdivide]

		sub		edi,eax

		//
		// eax = i_pixel
		// edi = inner loop count
		// ecx,edx,esi = texture values
		//
		mov		ebp,gsGlobals.pvScreen				// Pointer the screen.
		mov		ebx,[i_screen_index]				// Load scanline offset.

		add		ebx,eax								// Add scanline offset to i_pixel
		mov		[i_pixel],eax						// Save i_pixel.

		lea		ebx,[ebp+ebx*2]						// Base of span in ebx.
		mov		ebp,[w2dDeltaTex]CWalk2D.uVFrac		// Load V fraction step.

		//-----------------------------------------------------------------------
		// Draw the scanline.
		//
		// eax = temp for pixel
		// ebx = base of scanline
		// ecx = Ufrac
		// edx = UVint
		// esi = Vfrac
		// edi = offset & count
		// ebp = Vfrac step / v carry
		//
INNER_LOOP:
		add		ecx,ebp											// Step V fraction.
		mov		al,[edx*2]										// Read texture value (low).

		sbb		ebp,ebp											// Get borrow from V fraction step.
		mov		ah,[edx*2 + 1]									// Read texture value (high).

		add		esi,[w2dDeltaTex]CWalk2D.uUFrac					// Step U fraction.

		adc		edx,[w2dDeltaTex + ebp*4 + 4]CWalk2D.iUVInt		// Integer step + u carry.

		mov		[ebx + edi*2],ax								// Store pixel.
		add		edi,2											// Step 2 pixels

		mov		ebp,[w2dDeltaTex]CWalk2D.uVFrac					// Load V fraction step.
		jl		INNER_LOOP

		// See if there are any pixels left.
		cmp		[i_pixel],0
		je		END_OF_SCANLINE

		// Do perspective correction and looping.
		//PERSPECTIVE_SCANLINE_EPILOGUE_CLAMP

		//---------------------------------------------------------------------------------- 
		// Implement perspective correction. 
		// stack top contains the result of the divide 
		// Preserve: ecx,edx,esi 
		// 
		mov		edi,[iNextSubdivide]
		mov		eax,[iCacheNextSubdivide]

		sub		edi,eax
		jz		short EXIT_BEGIN_NEXT_QUICK_END

		fld		[fGUInvZ]					// U/Z,Z 
		fxch	st(1)						// Z,U/Z 
		fmul	st(1),st(0)					// Z,U 

		fmul	[fGVInvZ]					// V,U 

		//---------------------------------------------------------------------------------- 
		// Clamp U and V 
		fxch	st(1)
		fstp	[f_u]
		fstp	[f_v]

		mov		ebp,[f_u]
		mov		ebx,[fTexEdgeTolerance]
		mov		eax,[f_v]
		cmp		eax,ebx
		jge		short V_NOT_LESS_END
		mov		eax,ebx
V_NOT_LESS_END:
		cmp		ebp,ebx
		jge		short U_NOT_LESS_END
		mov		ebp,ebx
U_NOT_LESS_END:
		mov		ebx,[fTexWidth]
		cmp		ebp,ebx
		jle		short U_NOT_GREATER_END
		mov		ebp,ebx
U_NOT_GREATER_END:
		mov		ebx,[fTexHeight]
		cmp		eax,ebx
		jle		short V_NOT_GREATER_END
		mov		eax,ebx
V_NOT_GREATER_END:
		mov		[f_u],ebp
		mov		[f_v],eax

		fld		[f_u]
		fld		[f_v]

		//---------------------------------------------------------------------------------- 
		// Initialize walking values 
		fld		st(1)								// U,V,U 
		fsub	[fU]								// U-fU,V,U 
		fld		st(1)								// V,U-fU,V,U 
		fsub	[fV]								// V-fV,U-fU,V,U 
		fxch	st(1)								// U-fU,V-fV,V,U 
		fmul	float ptr[fInverseIntTable+edi*4]	// (U-fU)*C,V-fV,V,U 
		fxch	st(2)								// V,V-fV,(U-fU)*C,U 
		fstp	[fV]								// V-fV,(U-fU)*C,U 
		fmul	float ptr[fInverseIntTable+edi*4]	// (V-fV)*C,(U-fU)*C,U 
		fxch	st(1)								// (U-fU)*C,(V-fV)*C,U 
		fadd	[dFastFixed16Conversion]			// f(U-fU)*C,(V-fV)*C,U 
		fxch	st(1)								// (V-fV)*C,f(U-fU)*C,U 
		// stall(1) 
		fadd	[dFastFixed16Conversion]			// f(V-fV)*C,f(U-fU)*C,U 
		fxch	st(2)								// U,f(U-fU)*C,f(V-fV)*C 
		fstp	[fU]								// f(U-fU)*C,f(V-fV)*C 
		fstp	[d_temp_a]							// f(V-fV)*C 
		fstp	[d_temp_b]							

		mov		edi,dword ptr[d_temp_a]			// uslope 
		mov		eax,dword ptr[d_temp_b]			// vslope 

		sar		edi,15							// integer part of uslope 
		mov		ebp,dword ptr[d_temp_a]			// uslope again 

		shl		ebp,17							// fractional part of uslope 
		mov		ebx,eax							// vslope again 

		sar		eax,15							// integer part of vslope 
		mov		[w2dDeltaTex.uUFrac],ebp		// store UFrac 

		imul	eax,[iTexWidth]					// ivslope*twidth 

		shl		ebx,17							// fractional part of vslope 

		mov		[w2dDeltaTex.uVFrac],ebx		// store VFrac 

		add		edi,eax							// ivslope*twidth + iuslope 
		mov		ebp,[iTexWidth]					// Load texture width. 

		mov		[w2dDeltaTex.iUVInt+4],edi		// store integer stride 
		add		edi,ebp							// add twidth to integer stride 

		mov		[w2dDeltaTex.iUVInt],edi		// store integer stride + twidth 
		mov		eax,[iCacheNextSubdivide]

		//---------------------------------------------------------------------------------- 
		// Begin Next Subdivision 
		mov		[iNextSubdivide],eax			// eax == iNextSubdivide 
		mov		ebx,[iSubdivideLen]

		test	eax,eax							// Next subdivision is zero length? 
		jz		SUBDIVISION_LOOP

		// scan line is +ve 
		add		eax,ebx
		jle		short DONE_DIVIDE_PIXEL_CACHE_END

		// calc the new +ve ratio 
		fild	iNextSubdivide
		fld		fInvSubdivideLen
		fchs

		// Subdivision is smaller than iSubdivideLen 
		fmulp	st(1),st(0)			// st(0) = (-)fInvSubdivideLen * i_pixel; 
		fld		fDUInvZScanline		// U C 

		xor		eax,eax

		fmul	st(0),st(1)			// U*C C 
		fxch	st(1)				// C U*C 
		fld		fDVInvZScanline		// V C U*C 
		fxch	st(1)				// C V U*C 
		fmul	st(1),st(0)			// C V*C U*C 

		mov		[iCacheNextSubdivide],eax

		fmul	fDInvZScanline		// Z*C V*C U*C 
		fxch	st(2)				// U*C V*C Z*C 
		fst		fDUInvZScanline			// U V Z 
		fadd	fGUInvZ
		fxch	st(1)					// V U Z 
		fst		fDVInvZScanline
		fadd	fGVInvZ
		fxch	st(2)					// Z U V 
		fst		fDInvZScanline
		fadd	fGInvZ
		fxch	st(2)					// V U Z 
		fstp	fGVInvZ			
		fstp	fGUInvZ
		fst		fGInvZ
		fdivr	fOne					// Start the next division. 

		jmp		SUBDIVISION_LOOP

DONE_DIVIDE_PIXEL_CACHE_END:
		mov		[iCacheNextSubdivide],eax
		mov		ebx,[iNextSubdivide]

		cmp		eax,ebx
		je		SUBDIVISION_LOOP

		fld		fDUInvZScanline			// U 
		fadd	fGUInvZ
		fld		fDVInvZScanline			// V U 
		fadd	fGVInvZ
		fld		fDInvZScanline			// Z V U 
		fadd	fGInvZ
		fxch	st(2)					// U V Z 
		fstp	fGUInvZ
		fstp	fGVInvZ
		fst		fGInvZ
		fdivr	fOne					// Start the next division. 

		jmp		SUBDIVISION_LOOP

		// When the sub divide equals the cached sub-divide we end up here but 
		// there is an element left on the fp stack. This sould never happen. 
EXIT_BEGIN_NEXT_QUICK_END:

		// Dump value on stack 
		fcomp	st(0)
		jmp		SUBDIVISION_LOOP

END_OF_SCANLINE:
		pop		ebp						// Restore ebp.

		//
		// Increment the base edge.
		//
		mov		esi,[pdtri]							// Pointer to polygon object.
		mov		ecx,[pscan]							// Pointer to scanline object.

		// Step length, line starting address.
		mov		eax,[esi]CDrawPolygonBase.fxLineLength.i4Fx
		mov		ebx,[esi]CDrawPolygonBase.iLineStartIndex

		add		eax,[esi]CDrawPolygonBase.fxDeltaLineLength.i4Fx
		add		ebx,[gsGlobals.u4LinePixels]

		mov		[esi]CDrawPolygonBase.fxLineLength.i4Fx,eax
		mov		[esi]CDrawPolygonBase.iLineStartIndex,ebx

		// Step edge values.
		mov		eax,[ecx]TStippleTexPersp.fxX.i4Fx
		mov		ebx,[ecx]TStippleTexPersp.fxXDifference.i4Fx

		fld		[ecx]TStippleTexPersp.indCoord.fUInvZ
		fadd	[f_inc_uinvz]
		fld		[ecx]TStippleTexPersp.indCoord.fVInvZ
		fadd	[f_inc_vinvz]
		fld		[ecx]TStippleTexPersp.indCoord.fInvZ
		fadd	[f_inc_invz]

		add		eax,[fx_inc]
		add		ebx,[fx_diff]

		jge		short NO_UNDERFLOW

		// Underflow.
		add		ebx,0x00010000
		jmp		short BORROW_OR_CARRY

NO_UNDERFLOW:
		cmp		ebx,0x00010000
		jle		short NO_OVERFLOW

		// Overflow.
		sub		ebx,0x00010000
		jmp		short BORROW_OR_CARRY

NO_OVERFLOW:
		fstp	[ecx]TStippleTexPersp.indCoord.fInvZ
		fstp	[ecx]TStippleTexPersp.indCoord.fVInvZ
		fstp	[ecx]TStippleTexPersp.indCoord.fUInvZ

		jmp		short FINISH_LOOPING

BORROW_OR_CARRY:
		fadd	[fNegInvZ]								// Z V U
		fxch	st(1)									// V Z U
		fadd	[fNegVInvZ]								// V Z U
		fxch	st(2)									// U Z V
		fadd	[fNegUInvZ]								// U Z V
		fxch	st(1)									// Z U V
		fstp	[ecx]TStippleTexPersp.indCoord.fInvZ
		fstp	[ecx]TStippleTexPersp.indCoord.fUInvZ
		fstp	[ecx]TStippleTexPersp.indCoord.fVInvZ

FINISH_LOOPING:
		// Y Loop control.
		mov		[ecx]TStippleTexPersp.fxX.i4Fx,eax
		mov		edx,[esi]CDrawPolygonBase.iY

		mov		[ecx]TStippleTexPersp.fxXDifference.i4Fx,ebx
		inc		edx

		mov		ebx,[esi]CDrawPolygonBase.iYTo
		mov		[esi]CDrawPolygonBase.iY,edx

		cmp		edx,ebx
		jl		Y_LOOP
	}
}


//*****************************************************************************************
//
// Linear, stipple textured, 16 bit mode.
//
void DrawSubtriangle(TStippleTexLinear* pscan, CDrawPolygon<TStippleTexLinear>* pdtri)
{
	typedef TStippleTexLinear::TIndex prevent_internal_compiler_errors;
	static CDrawPolygon<TStippleTexLinear>* pdtriGlbl;
	static TStippleTexLinear* pscanGlbl;
	static CWalk2D w2dDeltaTex2;

	TStippleTexLinear* plinc = &pdtri->pedgeBase->lineIncrement;

	__asm
	{
		push	ebp									// Save ebp.
		mov		edi,[plinc]							// Pointer to scanline increment.

		mov		eax,[pscan]							// Pointer to scanline.
		mov		esi,[pdtri]							// Pointer to polygon.

		//
		// Copies of edge stepping values.
		//
		// fx_inc			= pdtri->pedgeBase->lineIncrement.fxX;
		// fx_diff			= pdtri->pedgeBase->lineIncrement.fxXDifference;
		// bf_u_inc			= pdtri->pedgeBase->lineIncrement.indCoord.bfU;
		// w1d_v_inc		= pdtri->pedgeBase->lineIncrement.indCoord.w1dV;
		// pscanGlbl		= pscan;
		// pdtriGlbl		= pdtri;
		//
		mov		ebx,[edi]TStippleTexLinear.fxX
		mov		ecx,[edi]TStippleTexLinear.fxXDifference

		mov		fx_inc,ebx
		mov		fx_diff,ecx

		mov		ebx,[edi]TStippleTexLinear.indCoord.bfU.i4Int
		mov		ecx,[edi]TStippleTexLinear.indCoord.bfU.u4Frac

		mov		bf_u_inc.i4Int,ebx
		mov		bf_u_inc.u4Frac,ecx

		mov		ebx,[edi]TStippleTexLinear.indCoord.w1dV.bfxValue.i4Int
		mov		ecx,[edi]TStippleTexLinear.indCoord.w1dV.bfxValue.u4Frac

		mov		w1d_v_inc.bfxValue.i4Int,ebx
		mov		w1d_v_inc.bfxValue.u4Frac,ecx

		mov		pdtriGlbl,esi
		mov		pscanGlbl,eax

		// Compute double step.
		mov		ebx,[bfDeltaU]UBigFixed.u4Frac

		and		ebx,0xffff0000

		add		ebx,ebx

		mov		[w2dDeltaTex2]CWalk2D.uUFrac,ebx

		mov		ecx,[bfDeltaU]UBigFixed.i4Int

		adc		ecx,ecx

		mov		ebx,[w1dDeltaV]CWalk1D.bfxValue.u4Frac

		and		ebx,0xffff0000

		mov		edx,[w1dDeltaV]CWalk1D.bfxValue.i4Int

		add		edx,edx

		add		ebx,ebx

		mov		[w2dDeltaTex2]CWalk2D.uVFrac,ebx
		mov		ebx,[w1dDeltaV]CWalk1D.iOffsetPerLine

		jnc		NO_V_CARRY

		add		edx,ebx

NO_V_CARRY:

		add		edx,ecx
		
		mov		[w2dDeltaTex2 + 4]CWalk2D.iUVInt,edx

		add		edx,ebx

		mov		[w2dDeltaTex2]CWalk2D.iUVInt,edx

		//
		// Iterate through the scanlines that intersect the subtriangle.
		//
Y_LOOP:
		// Check to see if we should skip this scanline.
		mov		ecx,[esi]CDrawPolygonBase.iY
		mov		ebx,[bEvenScanlinesOnly]

		and		ecx,ebx
		jnz		END_OF_SCANLINE

		// Determine the start and end of the scanline.
		mov		ebx,[eax]TStippleTexLinear.fxX.i4Fx
		mov		ecx,[esi]CDrawPolygonBase.fxLineLength.i4Fx

		mov		edx,[esi]CDrawPolygonBase.iLineStartIndex
		add		ebx,ecx

		sar		ebx,16
		mov		edi,[eax]TStippleTexLinear.fxX.i4Fx

		sar		edi,16
		add		edx,ebx

		sub		edi,ebx														// i_pixel
		jge		END_OF_SCANLINE

		//
		// edi = i_pixel
		// edx = i_screen_index
		// eax = pointer to scanline
		//
		mov		ebx,[eax]TStippleTexPersp.fxX.i4Fx							// Fractional X.
		mov		ebp,[esi]CDrawPolygonBase.iY								// Integer Y.

		sar		ebx,16														// Integer X.
		mov		esi,[eax]TStippleTexLinear.indCoord.bfU.u4Frac				// Ufrac

		xor		ebp,ebx														// XOR with integer X.
		mov		ebx,gsGlobals.pvScreen										// Pointer the screen.

		mov		ecx,[eax]TStippleTexLinear.indCoord.w1dV.bfxValue.u4Frac	// Vfrac

		lea		ebx,[ebx+edx*2]												// Base of scanline.
		mov		edx,[eax]TStippleTexLinear.indCoord.w1dV.bfxValue.i4Int		// Vint

		mov		eax,[eax]TStippleTexLinear.indCoord.bfU.i4Int				// Uint

		add		edx,eax														// Add Uint to Vint
		mov		eax,[pvTextureBitmap]										// Base of texture.
		
		shr		eax,1														// Word offset.

		add		edx,eax														// Add to start offset.

		// if ((y xor x) & 1) skip a pixel.
		test	ebp,1
		jz		NO_SKIP

		add		ecx,[w2dDeltaTex]CWalk2D.uVFrac					// Step V fraction.

		sbb		ebp,ebp											// Get borrow from V fraction step.
		add		esi,[w2dDeltaTex]CWalk2D.uUFrac					// Step U fraction.

		adc		edx,[w2dDeltaTex + ebp*4 + 4]CWalk2D.iUVInt		// Integer step + u carry.
		inc		edi

		jz		END_OF_SCANLINE

NO_SKIP:
		mov		ebp,[w2dDeltaTex2]CWalk2D.uVFrac				// Load V fraction step.

		//-----------------------------------------------------------------------
		// Draw the scanline.
		//
		// eax = temp for pixel
		// ebx = base of scanline
		// ecx = Ufrac
		// edx = UVint
		// esi = Vfrac
		// edi = offset & count
		// ebp = Vfrac step / v carry
		//
INNER_LOOP:
		add		ecx,ebp											// Step V fraction.
		mov		al,[edx*2]										// Read texture value (low).

		sbb		ebp,ebp											// Get borrow from V fraction step.
		mov		ah,[edx*2 + 1]									// Read texture value (high).

		add		esi,[w2dDeltaTex2]CWalk2D.uUFrac				// Step U fraction.

		adc		edx,[w2dDeltaTex2 + ebp*4 + 4]CWalk2D.iUVInt	// Integer step + u carry.

		mov		[ebx + edi*2],ax								// Store pixel.
		add		edi,2											// Step 2 pixels

		mov		ebp,[w2dDeltaTex2]CWalk2D.uVFrac				// Load V fraction step.
		jl		INNER_LOOP

END_OF_SCANLINE:
		mov		eax,[pscanGlbl]
		mov		ebp,bf_u_inc.i4Int

		// AGI on eax.

		mov		ecx,[eax]TStippleTexLinear.indCoord.bfU.u4Frac
		mov		edx,[eax]TStippleTexLinear.indCoord.bfU.i4Int

		mov		edi,[eax]TStippleTexLinear.indCoord.w1dV.bfxValue.i4Int
		mov		esi,[eax]TStippleTexLinear.indCoord.w1dV.bfxValue.u4Frac

		add		edi,w1d_v_inc.bfxValue.i4Int
		add		ecx,bf_u_inc.u4Frac

		adc		edx,ebp
		mov		ebp,w1d_v_inc.bfxValue.u4Frac

		add		esi,ebp
		jnc		short NO_VFRAC_CARRY

		add		edi,[eax]TStippleTexLinear.indCoord.w1dV.iOffsetPerLine

NO_VFRAC_CARRY:
		mov		ebx,[eax]TStippleTexLinear.fxXDifference
		mov		ebp,[fx_diff]

		add		ebx,ebp
		jge		short NO_UNDERFLOW

		// Undeflow of fxXDifference.
		add		ebx,0x00010000
		mov		ebp,bfNegDU.i4Int

		add		edi,w1dNegDV.bfxValue.i4Int
		add		ecx,bfNegDU.u4Frac

		adc		edx,ebp
		mov		ebp,w1dNegDV.bfxValue.u4Frac

		add		esi,ebp
		jnc		short EDGE_DONE

		add		edi,[eax]TStippleTexLinear.indCoord.w1dV.iOffsetPerLine
		jmp		short EDGE_DONE

NO_UNDERFLOW:
		cmp		ebx,0x00010000
		jl		short EDGE_DONE

		// Overflow of fxXDifference.
		sub		ebx,0x00010000
		mov		ebp,bfNegDU.i4Int

		add		edi,w1dNegDV.bfxValue.i4Int
		add		ecx,bfNegDU.u4Frac

		adc		edx,ebp
		mov		ebp,w1dNegDV.bfxValue.u4Frac

		add		esi,ebp
		jnc		short EDGE_DONE

		add		edi,[eax]TStippleTexLinear.indCoord.w1dV.iOffsetPerLine

EDGE_DONE:
		// Store modified variables and do looping.
		mov		[eax]TStippleTexLinear.indCoord.w1dV.bfxValue.u4Frac,esi
		mov		[eax]TStippleTexLinear.indCoord.w1dV.bfxValue.i4Int,edi

		mov		[eax]TStippleTexLinear.fxXDifference,ebx
		mov		esi,[pdtriGlbl]								// Pointer to polygon object.

		mov		[eax]TStippleTexLinear.indCoord.bfU.u4Frac,ecx
		mov		[eax]TStippleTexLinear.indCoord.bfU.i4Int,edx

		// Step length, line starting address.
		mov		ebx,[esi]CDrawPolygonBase.fxLineLength.i4Fx
		mov		ecx,[esi]CDrawPolygonBase.iLineStartIndex

		add		ebx,[esi]CDrawPolygonBase.fxDeltaLineLength.i4Fx
		add		ecx,[gsGlobals.u4LinePixels]

		mov		[esi]CDrawPolygonBase.fxLineLength.i4Fx,ebx
		mov		[esi]CDrawPolygonBase.iLineStartIndex,ecx

		mov		ecx,[eax]TStippleTexLinear.fxX
		mov		ebp,[fx_inc]

		add		ecx,ebp
		mov		edx,[esi]CDrawPolygonBase.iY

		mov		[eax]TStippleTexLinear.fxX,ecx
		inc		edx

		mov		ebx,[esi]CDrawPolygonBase.iYTo
		mov		[esi]CDrawPolygonBase.iY,edx

		cmp		edx,ebx
		jl		Y_LOOP

		pop ebp
	}
}


#if (0)

//*********************************************************************************************
void DrawSubtriangle(TWaterPersp* pscan, CDrawPolygon<TWaterPersp>* pdtri)
{
	typedef TWaterPersp::TPixel TDest;

	Assert(pscan);
	Assert(pdtri);

	int i_x_from;
	int i_x_to;
	int i_screen_index;
	int i_pixel;

	// Iterate through the scanlines that intersect the subtriangle.
	do
	{
		if (bRIGHT_TO_LEFT)
		{
			i_x_from = (pscan->fxX.i4Fx - pdtri->fxLineLength.i4Fx) >> 16;
			i_x_to   = pscan->fxX.i4Fx >> 16;
			i_screen_index = pdtri->iLineStartIndex + i_x_from - 1;
			i_pixel = i_x_to - i_x_from;
		}
		else
		{
			i_x_from = pscan->fxX.i4Fx >> 16;
			i_x_to   = (pscan->fxX.i4Fx + pdtri->fxLineLength.i4Fx) >> 16;
			i_screen_index = pdtri->iLineStartIndex + i_x_to;
			i_pixel = i_x_from - i_x_to;
		}

		// Draw if there are pixels to draw.
		if (i_x_to > i_x_from && !(bEvenScanlinesOnly && ((pdtri->iY & 1) == 0)))
		{
			{
				TWaterPersp::TIndex index(pscan->indCoord, i_pixel);
				TDest* apix_screen = ((TDest*)pdtri->prasScreen->pSurface) + i_screen_index;

				// Iterate left or right along the destination scanline.
				for (;;)
				{
					// Do next subdivision.
					int i_pixel_inner = index.i4StartSubdivision(i_pixel);

					TDest* apix_screen_inner = apix_screen + i_pixel;

					for (;;)
					{
						
						// Get an indexed alpha pixel.
						uint16 u2_alpha = ((uint16*)pvTextureBitmap)[index.iGetIndex()];

						++index;

						// Get the screen pixel.
						uint16 u2_screen = apix_screen_inner[i_pixel_inner];

						// And off bits for the alpha mask.
						u2_screen &= u2WaterMask;

						// Or in bits for the alpha band.
						u2_screen |= u2_alpha;

						// Look up the alpha colour.
						u2_screen = pu2WaterAlpha[u2_screen];

						// Write the pixel back to the screen.
						apix_screen_inner[i_pixel_inner] = u2_screen;

						// Increment values.
						i_pixel_inner += i4ScanlineDirection;

						// Break from subdivision loop if there are no more pixels to render.
						if (i_pixel_inner == 0)
							break;
					}

					// Exit scanline routine if there are no more pixels to render.
					if (i_pixel == 0)
						break;

					// Implement perspective correction.
					index.UpdatePerspective(0);
				}
			}
		}
		
		// Increment the base edge.
		++*pdtri->pedgeBase;

		// Set the new length of the line to be rasterized.
		pdtri->fxLineLength.i4Fx += pdtri->fxDeltaLineLength.i4Fx;

		// Get new starting pixel TIndex for the scanline.
		pdtri->iLineStartIndex += pdtri->prasScreen->iLinePixels;
	}
	while (++pdtri->iY < pdtri->iYTo);
}

//*********************************************************************************************
void DrawSubtriangle(TWaterLinear* pscan, CDrawPolygon<TWaterLinear>* pdtri)
{
	typedef TWaterLinear::TPixel TDest;

	Assert(pscan);
	Assert(pdtri);

	int i_x_from;
	int i_x_to;
	int i_screen_index;
	int i_pixel;

	// Iterate through the scanlines that intersect the subtriangle.
	do
	{
		if (bRIGHT_TO_LEFT)
		{
			i_x_from = (pscan->fxX.i4Fx - pdtri->fxLineLength.i4Fx) >> 16;
			i_x_to   = pscan->fxX.i4Fx >> 16;
			i_screen_index = pdtri->iLineStartIndex + i_x_from - 1;
			i_pixel = i_x_to - i_x_from;
		}
		else
		{
			i_x_from = pscan->fxX.i4Fx >> 16;
			i_x_to   = (pscan->fxX.i4Fx + pdtri->fxLineLength.i4Fx) >> 16;
			i_screen_index = pdtri->iLineStartIndex + i_x_to;
			i_pixel = i_x_from - i_x_to;
		}

		// Draw if there are pixels to draw.
		if (i_x_to > i_x_from && !(bEvenScanlinesOnly && ((pdtri->iY & 1) == 0)))
		{
			{
				TWaterLinear::TIndex index(pscan->indCoord, i_pixel);
				TDest* apix_screen = ((TDest*)pdtri->prasScreen->pSurface) + i_screen_index;

				// Iterate left or right along the destination scanline.
				for (;;)
				{
					// Do next subdivision.
					int i_pixel_inner = index.i4StartSubdivision(i_pixel);

					TDest* apix_screen_inner = apix_screen + i_pixel;

					for (;;)
					{
						
						// Get an indexed alpha pixel.
						uint16 u2_alpha = ((uint16*)pvTextureBitmap)[index.iGetIndex()];

						++index;

						// Get the screen pixel.
						uint16 u2_screen = apix_screen_inner[i_pixel_inner];

						// And off bits for the alpha mask.
						u2_screen &= u2WaterMask;

						// Or in bits for the alpha band.
						u2_screen |= u2_alpha;

						// Look up the alpha colour.
						u2_screen = pu2WaterAlpha[u2_screen];

						// Write the pixel back to the screen.
						apix_screen_inner[i_pixel_inner] = u2_screen;

						// Increment values.
						i_pixel_inner += i4ScanlineDirection;

						// Break from subdivision loop if there are no more pixels to render.
						if (i_pixel_inner == 0)
							break;
					}

					// Exit scanline routine if there are no more pixels to render.
					if (i_pixel == 0)
						break;

					// Implement perspective correction.
					index.UpdatePerspective(0);
				}
			}
		}
		
		// Increment the base edge.
		++*pdtri->pedgeBase;

		// Set the new length of the line to be rasterized.
		pdtri->fxLineLength.i4Fx += pdtri->fxDeltaLineLength.i4Fx;

		// Get new starting pixel TIndex for the scanline.
		pdtri->iLineStartIndex += pdtri->prasScreen->iLinePixels;
	}
	while (++pdtri->iY < pdtri->iYTo);
}

#endif
